#include <Graphics/graphicsContext.hpp>

#include <editor.hpp>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlgpu3.h>

void ImDrawCallback_ImplSDLGPU3_SetSampler (const ImDrawList* /*parent_list*/, const ImDrawCmd* cmd)
{
    ImGui_ImplSDLGPU3_RenderState* state = (ImGui_ImplSDLGPU3_RenderState*) ImGui::GetPlatformIO().Renderer_RenderState;
    SDL_GPUSampler* sampler = cmd->UserCallbackData ? (SDL_GPUSampler*) cmd->UserCallbackData : state->SamplerNearest;
    state->SamplerCurrent = sampler;
}

void Editor::initImGuiContext (GraphicsContext* context)
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
    initInfo.PresentMode = SDL_GPU_PRESENTMODE_IMMEDIATE;
    ImGui_ImplSDLGPU3_Init (&initInfo);

    ImGuiStyle& style = ImGui::GetStyle();

    //Handle High DPI mode.
    float mainScale = SDL_GetDisplayContentScale (SDL_GetPrimaryDisplay());
    style.ScaleAllSizes (mainScale);
    style.FontScaleDpi = mainScale;
}

void Editor::handleEvents (SDL_Event* event)
{
    ImGui_ImplSDL3_ProcessEvent (event);
}

void Editor::startFrame()
{
    ImGui_ImplSDLGPU3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport();
}

void Editor::drawEditor (GraphicsContext* context)
{
    ImGui::Begin ("Graphics Context");

    auto io = ImGui::GetIO();
    ImGui::Text ("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::Text ("Driver Name: %s", SDL_GetGPUDeviceDriver (context->getDevice()));
    auto properties = SDL_GetGPUDeviceProperties (context->getDevice());
    (SDL_EnumerateProperties (properties, [] (void*, SDL_PropertiesID prop, const char* name)
                              {
        ImGui::Text ("%s", name);
        ImGui::SameLine();
        ImGui::GetWindowDrawList()->AddCallback (ImDrawCallback_ImplSDLGPU3_SetSampler, nullptr);
        ImGui::Text ("%s", SDL_GetStringProperty (prop, name, "")); },
                              nullptr));

    ImGui::End();

    ImGui::Begin ("GameWindow");
    ImGui::GetWindowDrawList()->AddCallback (ImDrawCallback_ImplSDLGPU3_SetSampler, nullptr);
    auto windowWidth = ImGui::GetWindowSize().x;
    auto windowHeight = ImGui::GetWindowSize().y;
    {
        ImGui::SetCursorPos ({ (windowWidth - 640) / 2, (windowHeight - 480) / 2 });
        ImGui::Image (context->getRenderTexture(), { 640, 480 });
    }

    ImGui::End();
}

void Editor::endFrame (FrameContext* frameContext)
{
    ImGui::EndFrame();
    ImGui::UpdatePlatformWindows();
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
}