#include "graphicsContext.hpp"
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlgpu3.h>

#include <iostream>

GraphicsContext::GraphicsContext()
{

}

void GraphicsContext::initContext(SDL_Window* windowIn)
{
    window = windowIn;

    device = SDL_CreateGPUDevice(
        SDL_GPU_SHADERFORMAT_MSL | SDL_GPU_SHADERFORMAT_SPIRV,
        true,
        nullptr
    );
    if(device == nullptr)
    {
        std::cerr << "SDL GPU Device Creation failed: "  << SDL_GetError() << std::endl;
        exit(-1);
    }
    if(!SDL_ClaimWindowForGPUDevice(device,window))
    {
        std::cerr << "SDL GPU Device Window Claim failed: "  << SDL_GetError() << std::endl;
        exit(-1);
    }

    if(!SDL_SetGPUSwapchainParameters(device, window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_VSYNC))
    {
        std::cerr << "SDL GPU SwapChain setup failed: "  << SDL_GetError() << std::endl;
        exit(-1);
    }
    
}

void GraphicsContext::initImGuiGPU()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    //Create a Dockable ImGUI Instance
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


    ImGui_ImplSDL3_InitForSDLGPU(window);
    ImGui_ImplSDLGPU3_InitInfo initInfo = {};
    initInfo.Device = device;
    initInfo.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(device, window);
    //initInfo.MSAASamples = SDL_GPU_SAMPLECOUNT_1;                      // Only used in multi-viewports mode.
    //initInfo.SwapchainComposition = SDL_GPU_SWAPCHAINCOMPOSITION_SDR;  // Only used in multi-viewports mode.
    initInfo.PresentMode = SDL_GPU_PRESENTMODE_VSYNC;
    ImGui_ImplSDLGPU3_Init(&initInfo);


        ImGui::StyleColorsLight();
    ImGuiStyle& style = ImGui::GetStyle();

    //Handle High DPI mode.
    float mainScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    style.ScaleAllSizes(mainScale);
    style.FontScaleDpi = mainScale;
}

void GraphicsContext::debugView()
{
    ImGui::Begin("debug");
    {
            ImGuiIO& io = ImGui::GetIO();
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    }
    ImGui::End();
    ImGui::Begin("Graphics Context");
    ImGui::Text("Driver Name: %s", SDL_GetGPUDeviceDriver(device));
    auto properties = SDL_GetGPUDeviceProperties(device);
    (SDL_EnumerateProperties(properties,[](void* userdata, SDL_PropertiesID prop, const char* name){
        ImGui::Text("%s", name);
        ImGui::SameLine();
        ImGui::Text("%s",SDL_GetStringProperty(prop,name,""));
    },nullptr));

    ImGui::End();
}

void GraphicsContext::startFrame()
{
        ImGui_ImplSDLGPU3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();
}

void GraphicsContext::endFrame()
{
        ImGui::EndFrame();
        ImGui::UpdatePlatformWindows();
        // Rendering
        ImGui::Render();
        ImDrawData* draw_data = ImGui::GetDrawData();
        const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

        SDL_GPUCommandBuffer* command_buffer = SDL_AcquireGPUCommandBuffer(device); // Acquire a GPU command buffer

        SDL_GPUTexture* swapchain_texture;
        SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, window, &swapchain_texture, nullptr, nullptr); // Acquire a swapchain texture

        if (swapchain_texture != nullptr && !is_minimized)
        {
            // This is mandatory: call ImGui_ImplSDLGPU3_PrepareDrawData() to upload the vertex/index buffer!
            ImGui_ImplSDLGPU3_PrepareDrawData(draw_data, command_buffer);

            // Setup and start a render pass
            SDL_GPUColorTargetInfo target_info = {};
            target_info.texture = swapchain_texture;
            target_info.clear_color = SDL_FColor { 0,0,0,0};
            target_info.load_op = SDL_GPU_LOADOP_CLEAR;
            target_info.store_op = SDL_GPU_STOREOP_STORE;
            target_info.mip_level = 0;
            target_info.layer_or_depth_plane = 0;
            target_info.cycle = false;
            SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(command_buffer, &target_info, 1, nullptr);

            // Render ImGui
            ImGui_ImplSDLGPU3_RenderDrawData(draw_data, command_buffer, render_pass);

            SDL_EndGPURenderPass(render_pass);
        }

        // Submit the command buffer
        SDL_SubmitGPUCommandBuffer(command_buffer);
}
