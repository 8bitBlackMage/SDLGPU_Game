#include <Imgui/imguiHelpers.hpp>
#include <Imgui/imguiTheme.hpp>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlgpu3.h>
void ImDrawCallback_ImplSDLGPU3_SetSampler (const ImDrawList* /*parent_list*/, const ImDrawCmd* cmd)
{
    ImGui_ImplSDLGPU3_RenderState* state = (ImGui_ImplSDLGPU3_RenderState*) ImGui::GetPlatformIO().Renderer_RenderState;
    SDL_GPUSampler* sampler = cmd->UserCallbackData ? (SDL_GPUSampler*) cmd->UserCallbackData : state->SamplerNearest;
    state->SamplerCurrent = sampler;
}

namespace ImGUIHelpers
{
void initContext (GraphicsContext* context)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    //Create a Dockable ImGUI Instance
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    io.Fonts->AddFontFromFileTTF ("assets/fonts/NotoSans-VariableFont_wdth,wght.ttf", 18.f);

    ImGui_ImplSDL3_InitForSDLGPU (context->getWindow());
    ImGui_ImplSDLGPU3_InitInfo initInfo = {};
    initInfo.Device = context->getDevice();
    initInfo.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat (context->getDevice(), context->getWindow());
    initInfo.PresentMode = SDL_GPU_PRESENTMODE_VSYNC;
    initInfo.MSAASamples = SDL_GPU_SAMPLECOUNT_1; // Only used in multi-viewports mode.
    initInfo.SwapchainComposition = SDL_GPU_SWAPCHAINCOMPOSITION_SDR; // Only used in multi-viewports mode.
    ImGui_ImplSDLGPU3_Init (&initInfo);

    ImGuiStyle& style = ImGui::GetStyle();
    io.ConfigDpiScaleFonts = true; // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
    io.ConfigDpiScaleViewports = true; // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.

    //Handle High DPI mode.
    float mainScale = SDL_GetDisplayContentScale (SDL_GetPrimaryDisplay());
    style.ScaleAllSizes (mainScale);
    style.FontScaleDpi = mainScale;
}

void startFrame()
{
    styleLayout();
    styleColourDark();
    ImGui_ImplSDLGPU3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport();
}

void handleEvents (SDL_Event* event)
{
    ImGui_ImplSDL3_ProcessEvent (event);
}

void endFrame (FrameContext* frameContext)
{
    ImGui::EndFrame();

    // Rendering
    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();
    const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
    if (frameContext->swapchainTexture != nullptr && ! is_minimized)
    {
        // This is mandatory: call ImGui_ImplSDLGPU3_PrepareDrawData() to upload the vertex/index buffer!
        ImGui_ImplSDLGPU3_PrepareDrawData (draw_data, frameContext->commandBuffer);

        SDL_GPUColorTargetInfo target_info = {};
        target_info.texture = frameContext->swapchainTexture;
        target_info.clear_color = SDL_FColor { 0, 0, 0, 0 };
        target_info.load_op = SDL_GPU_LOADOP_CLEAR;
        target_info.store_op = SDL_GPU_STOREOP_STORE;
        target_info.mip_level = 0;
        target_info.layer_or_depth_plane = 0;
        target_info.cycle = true;
        SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass (frameContext->commandBuffer, &target_info, 1, nullptr);

        //Render ImGui
        ImGui_ImplSDLGPU3_RenderDrawData (draw_data, frameContext->commandBuffer, render_pass);

        SDL_EndGPURenderPass (render_pass);
    }
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

} // namespace ImGUIHelpers