#include "Engine/Graphics/Textures/textureManager.hpp"
#include "Engine/Utils/matricies.hpp"
#include "SDL3/SDL_error.h"
#include <Engine/Graphics/graphicsContext.hpp>

#include <Engine/Utils/logger.hpp>
#include <SDL3_image/SDL_image.h>
#include <filesystem>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlgpu3.h>

void ImDrawCallback_ImplSDLGPU3_SetSampler (const ImDrawList* parent_list, const ImDrawCmd* cmd)
{
    ImGui_ImplSDLGPU3_RenderState* state = (ImGui_ImplSDLGPU3_RenderState*) ImGui::GetPlatformIO().Renderer_RenderState;
    SDL_GPUSampler* sampler = cmd->UserCallbackData ? (SDL_GPUSampler*) cmd->UserCallbackData : state->SamplerNearest;
    state->SamplerCurrent = sampler;
}

GraphicsContext::GraphicsContext()
{
}

GraphicsContext::~GraphicsContext()
{
    shutdown();
}

void GraphicsContext::initContext (SDL_Window* windowIn)
{
    window = windowIn;

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

    auto samplerCreateInfo = (SDL_GPUSamplerCreateInfo) {
        .min_filter = SDL_GPU_FILTER_NEAREST,
        .max_anisotropy = SDL_GPU_FILTER_NEAREST,
        .mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
        .address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
        .address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
        .address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
    };

    auto textureFormat = SDL_GetGPUSwapchainTextureFormat (device, window);
    auto textureCreateInfo = (SDL_GPUTextureCreateInfo) {
        .height = 480,
        .width = 640,
        .num_levels = 1,
        .layer_count_or_depth = 1,
        .format = textureFormat,
        .sample_count = SDL_GPU_SAMPLECOUNT_1,
        .type = SDL_GPU_TEXTURETYPE_2D,
        .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER
    };
    renderTexture = SDL_CreateGPUTexture (device, &textureCreateInfo);

    sampler = SDL_CreateGPUSampler (device, &samplerCreateInfo);

    frameContext.cameraMatrix = Matrix4x4::CreateOrthographicOffCenter (
        0,
        320,
        240,
        0,
        0,
        -1);
}

void GraphicsContext::shutdown()
{
    SDL_ReleaseGPUSampler (device, sampler);
    SDL_ReleaseGPUTexture (device, renderTexture);
}

void GraphicsContext::initImGuiGPU()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    //Create a Dockable ImGUI Instance
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    io.Fonts->AddFontFromFileTTF ("assets/fonts/NotoSans-VariableFont_wdth,wght.ttf", 18.f);

    ImGui_ImplSDL3_InitForSDLGPU (window);
    ImGui_ImplSDLGPU3_InitInfo initInfo = {};
    initInfo.Device = device;
    initInfo.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat (device, window);
    initInfo.PresentMode = SDL_GPU_PRESENTMODE_VSYNC;
    ImGui_ImplSDLGPU3_Init (&initInfo);

    ImGuiStyle& style = ImGui::GetStyle();

    //Handle High DPI mode.
    float mainScale = SDL_GetDisplayContentScale (SDL_GetPrimaryDisplay());
    style.ScaleAllSizes (mainScale);
    style.FontScaleDpi = mainScale;
}

void GraphicsContext::debugView()
{
    ImGui::Begin ("Graphics Context");

    auto io = ImGui::GetIO();
    ImGui::Text ("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::Text ("Driver Name: %s", SDL_GetGPUDeviceDriver (device));
    auto properties = SDL_GetGPUDeviceProperties (device);
    (SDL_EnumerateProperties (properties, [] (void* userdata, SDL_PropertiesID prop, const char* name)
                              {
        ImGui::Text ("%s", name);
        ImGui::SameLine();
        ImGui::GetWindowDrawList()->AddCallback (ImDrawCallback_ImplSDLGPU3_SetSampler, nullptr);
        ImGui::Text ("%s", SDL_GetStringProperty (prop, name, "")); },
                              nullptr));

    ImGui::End();
    textureManager.debugView();
}

void GraphicsContext::startFrame()
{
    ImGui_ImplSDLGPU3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport();
    ImGui::Begin ("GameWindow");
    ImGui::GetWindowDrawList()->AddCallback (ImDrawCallback_ImplSDLGPU3_SetSampler, nullptr);

    frameContext.device = device;
    frameContext.commandBuffer = SDL_AcquireGPUCommandBuffer (device); // Acquire a GPU command buffer
    frameContext.textureManager = &textureManager;
    frameContext.swapchainTexture = renderTexture;

    auto windowWidth = ImGui::GetWindowSize().x;
    auto windowHeight = ImGui::GetWindowSize().y;
    {
        ImGui::SetCursorPos ({ (windowWidth - 640) / 2, (windowHeight - 480) / 2 });
        ImGui::Image (renderTexture, { 640, 480 });
    }

    ImGui::End();
}

void GraphicsContext::endFrame()
{
    SDL_WaitAndAcquireGPUSwapchainTexture (frameContext.commandBuffer, window, &frameContext.swapchainTexture, nullptr, nullptr); // Acquire a swapchain texture
    ImGui::EndFrame();
    ImGui::UpdatePlatformWindows();
    // Rendering
    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();
    const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
    if (frameContext.swapchainTexture != nullptr && ! is_minimized)
    {
        // This is mandatory: call ImGui_ImplSDLGPU3_PrepareDrawData() to upload the vertex/index buffer!
        ImGui_ImplSDLGPU3_PrepareDrawData (draw_data, frameContext.commandBuffer);

        SDL_GPUColorTargetInfo target_info = {};
        target_info.texture = frameContext.swapchainTexture;
        target_info.clear_color = SDL_FColor { 0, 0, 0, 0 };
        target_info.load_op = SDL_GPU_LOADOP_CLEAR;
        target_info.store_op = SDL_GPU_STOREOP_STORE;
        target_info.mip_level = 0;
        target_info.layer_or_depth_plane = 0;
        target_info.cycle = true;
        SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass (frameContext.commandBuffer, &target_info, 1, nullptr);

        //Render ImGui
        ImGui_ImplSDLGPU3_RenderDrawData (draw_data, frameContext.commandBuffer, render_pass);

        SDL_EndGPURenderPass (render_pass);
    }

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
