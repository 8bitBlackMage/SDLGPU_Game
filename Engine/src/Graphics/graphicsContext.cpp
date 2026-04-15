

#include "Graphics/camera.hpp"
#include "SDL3/SDL_gpu.h"
#include "Utils/fileHelpers.hpp"
#include <Graphics/Textures/textureManager.hpp>
#include <Graphics/graphicsContext.hpp>
#include <Graphics/renderTexture.hpp>
#include <Utils/logger.hpp>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <filesystem>
#include <glm/gtc/matrix_transform.hpp>

GraphicsContext::GraphicsContext()
{
}

GraphicsContext::~GraphicsContext()
{
    shutdown();
}

void GraphicsContext::initContext (const std::string& windowName)
{
    window = SDL_CreateWindow (windowName.c_str(), 1920, 1080, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_HIDDEN);
    if (window == nullptr)
    {
        Logger::log ("SDL WINDOW Creation failed: ", SDL_GetError());
        exit (-1);
    }
    device = SDL_CreateGPUDevice (
        SDL_GPU_SHADERFORMAT_MSL | SDL_GPU_SHADERFORMAT_SPIRV,
        true,
        nullptr);
    if (device == nullptr)
    {
        Logger::log ("SDL GPU Device Creation failed:", SDL_GetError());
        exit (-1);
    }
    if (! SDL_ClaimWindowForGPUDevice (device, window))
    {
        Logger::log ("SDL GPU Device Window Claim failed: ", SDL_GetError());
        exit (-1);
    }
    else
    {
        Logger::log ("Created Device:", SDL_GetGPUDeviceDriver (device));
    }

    if (! SDL_SetGPUSwapchainParameters (device, window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_VSYNC))
    {
        Logger::log ("SDL GPU SwapChain setup failed: ", SDL_GetError());
        exit (-1);
    }

    auto samplerCreateInfo = SDL_GPUSamplerCreateInfo {
        .min_filter = SDL_GPU_FILTER_NEAREST,
        .max_anisotropy = SDL_GPU_FILTER_NEAREST,
        .mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
        .address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
        .address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
        .address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
    };

    sampler = SDL_CreateGPUSampler (device, &samplerCreateInfo);
    SDL_ShowWindow (window);
}

void GraphicsContext::shutdown()
{
    SDL_ReleaseGPUSampler (device, sampler);
}

void GraphicsContext::setCamera (Camera* camera)
{
    currentCamera = camera;
}

void GraphicsContext::startFrame()
{
    frameContext.window = window;
    frameContext.device = device;
    frameContext.commandBuffer = SDL_AcquireGPUCommandBuffer (device); // Acquire a GPU command buffer
    frameContext.textureManager = &textureManager;
    SDL_WaitAndAcquireGPUSwapchainTexture (frameContext.commandBuffer, window, &swapChainTexture, nullptr, nullptr); // Acquire a swapchain texture

    int deviceW, deviceH;

    SDL_GetWindowSizeInPixels (window, &deviceW, &deviceH);
    windowSize.x = (float) deviceW;
    windowSize.y = (float) deviceH;

    calculateViews (windowSize.x, windowSize.y);

    frameContext.swapchainTexture = swapChainTexture;
}

void GraphicsContext::startRenderTexture (RenderTexture* texture)
{
    currentRenderTexture = texture;
    frameContext.swapchainTexture = texture->getTexture();
    calculateViews (texture->getSize().x, texture->getSize().y);
}

void GraphicsContext::endRenderTexture()
{
    currentRenderTexture = nullptr;
    frameContext.swapchainTexture = swapChainTexture;
    calculateViews (windowSize.x, windowSize.y);
}

void GraphicsContext::endFrame()
{
    // Submit the command buffer
    SDL_SubmitGPUCommandBuffer (frameContext.commandBuffer);
}

SDL_GPUShader* GraphicsContext::loadShader (std::string filename, Uint32 samplerCount, Uint32 uniformBufferCount, Uint32 storageBufferCount, Uint32 storageTextureCount)
{
    Logger::getLogger().appendToLog ("Loading Shader from:", filename.c_str());
    if (! device)
    {
        Logger::getLogger().appendToLog ("Device not initialised");
        return nullptr;
    }

    SDL_GPUShaderStage stage;
    if (filename.contains (".vert"))
    {
        stage = SDL_GPU_SHADERSTAGE_VERTEX;
    }
    else if (filename.contains (".frag"))
    {
        stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
    }
    else
    {
        Logger::getLogger().appendToLog ("invalid shader type");
        return nullptr;
    }

    std::filesystem::path shaderpath;

    SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats (device);
    SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
    std::string entryPoint;
    if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV)
    {
        format = SDL_GPU_SHADERFORMAT_SPIRV;
        entryPoint = "main";
        shaderpath = getAssetFolderPath().string() + "/shaders/compiled/SPIRV/" + filename + ".spv";
    }
    else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL)
    {
        format = SDL_GPU_SHADERFORMAT_MSL;
        entryPoint = "main0";
        shaderpath = getAssetFolderPath().string() + "/shaders/compiled/MSL/" + filename + ".msl";
    }
    else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL)
    {
        format = SDL_GPU_SHADERFORMAT_DXIL;
        entryPoint = "main";
        shaderpath = getAssetFolderPath().string() + "/shaders/compiled/DXIL/" + filename + ".dxil";
    }
    else
    {
        Logger::getLogger().appendToLog ("Invalid shader format");
        return nullptr;
    }

    size_t codeSize;
    void* code = SDL_LoadFile (shaderpath.c_str(), &codeSize);

    if (code == nullptr)
    {
        Logger::log ("Failed to load shader from disk", SDL_GetError());
        return nullptr;
    }

    SDL_GPUShaderCreateInfo createInfo;

    createInfo.code = static_cast<const uint8_t*> (code);
    createInfo.code_size = codeSize;
    createInfo.entrypoint = entryPoint.c_str();
    createInfo.format = format;
    createInfo.stage = stage;
    createInfo.num_samplers = samplerCount;
    createInfo.num_uniform_buffers = uniformBufferCount;
    createInfo.num_storage_buffers = storageBufferCount;
    createInfo.num_storage_textures = storageTextureCount;

    SDL_GPUShader* shader = SDL_CreateGPUShader (device, &createInfo);
    if (shader == nullptr)
    {
        Logger::log ("failed to create shader", SDL_GetError);
        SDL_free (code);
        return nullptr;
    }

    SDL_free (code);

    return shader;
}

void GraphicsContext::calculateViews (float width, float height)
{
    float virtualW;
    float virtualH;

    if (currentCamera != nullptr)
    {
        virtualW = currentCamera->getWidth();
        virtualH = currentCamera->getHeight();
    }
    else
    {
        virtualW = width;
        virtualH = height;
    }

    float deviceRatio = width / height;
    float virtualRatio = virtualW / virtualH;

    float xScale = width / virtualW;
    float yScale = height / virtualH;
    float scale;
    if (virtualRatio < deviceRatio)
    {
        scale = floor (yScale);
    }
    else
    {
        scale = floor (xScale);
    }

    float correctedW = virtualW * scale;
    float correctedH = virtualH * scale;
    float correctedX = (width - correctedW) / 2.0f;
    float correctedY = (height - correctedH) / 2.0f;
    frameContext.viewport = { correctedX, correctedY, correctedW, correctedH };
    if (currentCamera == nullptr)
    {
        frameContext.cameraMatrix = glm::ortho (correctedX, correctedW, correctedH, correctedY);
    }
    else
    {
        frameContext.cameraMatrix = currentCamera->getCurrentMatrix();
    }
}