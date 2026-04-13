

#include <Graphics/Textures/textureManager.hpp>
#include <Graphics/graphicsContext.hpp>
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

void GraphicsContext::initContext()
{
    window = SDL_CreateWindow ("Dungeon Crawler", 1920, 1080, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_HIDDEN);
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

    if (! SDL_SetGPUSwapchainParameters (device, window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_IMMEDIATE))
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

bool GraphicsContext::setRenderTextureSize (float w, float h)
{
    auto textureFormat = SDL_GetGPUSwapchainTextureFormat (device, window);
    auto textureCreateInfo = SDL_GPUTextureCreateInfo {
        .height = static_cast<Uint32> (h),
        .width = static_cast<Uint32> (w),
        .num_levels = 1,
        .layer_count_or_depth = 1,
        .format = textureFormat,
        .sample_count = SDL_GPU_SAMPLECOUNT_1,
        .type = SDL_GPU_TEXTURETYPE_2D,
        .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET
    };
    renderTexture = SDL_CreateGPUTexture (device, &textureCreateInfo);
    if (renderTexture == nullptr)
    {
        return false;
    }
    renderTextureSize.x = w;
    renderTextureSize.y = h;

    return true;
}

void GraphicsContext::shutdown()
{
    SDL_ReleaseGPUSampler (device, sampler);
    SDL_ReleaseGPUTexture (device, renderTexture);
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

    float deviceRatio = (float) deviceW / (float) deviceH;
    float virtualRatio = 320.f / 240.f;

    float xScale = deviceW / 320.f;
    float yScale = deviceH / 240.f;
    float scale;
    if (virtualRatio < deviceRatio)
    {
        scale = floor (yScale);
    }
    else
    {
        scale = floor (xScale);
    }

    float correctedW = 320 * scale;
    float correctedH = 240 * scale;
    float correctedX = (deviceW - correctedW) / 2.0f;
    float correctedY = (deviceH - correctedH) / 2.0f;
    frameContext.viewport = { correctedX, correctedY, correctedW, correctedH };
    frameContext.swapchainTexture = swapChainTexture;
}

void GraphicsContext::endFrame()
{
    // Submit the command buffer
    SDL_SubmitGPUCommandBuffer (frameContext.commandBuffer);
}

void GraphicsContext::startRenderTexture()
{
    frameContext.swapchainTexture = renderTexture;
}

void GraphicsContext::endRenderTexture()
{
    frameContext.swapchainTexture = swapChainTexture;
}

void GraphicsContext::blitRenderTexture()
{
    int windowWidth, windowHeight;
    SDL_GetWindowSize (window, &windowWidth, &windowHeight);
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
        shaderpath = "assets/shaders/compiled/SPIRV/" + filename + ".spv";
    }
    else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL)
    {
        format = SDL_GPU_SHADERFORMAT_MSL;
        entryPoint = "main0";
        shaderpath = "assets/shaders/compiled/MSL/" + filename + ".msl";
    }
    else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL)
    {
        format = SDL_GPU_SHADERFORMAT_DXIL;
        entryPoint = "main";
        shaderpath = "assets/shaders/compiled/DXIL/" + filename + ".dxil";
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
