
#include "SDL3/SDL_events.h"
#include "Utils/logger.hpp"
#include "editorSettings.hpp"

#include "imgui_internal.h"
#include "sceneEditor.hpp"
#include <Graphics/graphicsContext.hpp>
#include <Imgui/imguiHelpers.hpp>

#include <Utils/fileHelpers.hpp>
#include <imgui.h>

int main()
{
    Logger::log (getSettingsFolderPath());
    Scene scene;
    scene.width = 320;
    scene.height = 240;
    scene.gridSize = 32;

    bool running = true;
    bool showLogger = false;
    GraphicsContext context;
    SceneEditor editor;
    EditorSettings editorSettings;

    context.initContext();
    editor.init (&scene, &context);
    ImGUIHelpers::initContext (&context);

    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent (&event))
        {
            ImGUIHelpers::handleEvents (&event);
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        context.startFrame();
        auto id = ImGUIHelpers::startFrame();
        static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoWindowMenuButton;
        static auto first_time = true;
        if (first_time)
        {
            first_time = false;
            ImGui::DockBuilderRemoveNode (id);
            ImGui::DockBuilderAddNode (id, dockspaceFlags);
            auto left = ImGui::DockBuilderSplitNode (id, ImGuiDir_Left, 0.2f, nullptr, &id);
            auto right = ImGui::DockBuilderSplitNode (id, ImGuiDir_Right, 0.2f, nullptr, &id);
            auto bottom = ImGui::DockBuilderSplitNode (id, ImGuiDir_Down, 0.3f, nullptr, &id);

            ImGui::DockBuilderDockWindow ("Log", bottom);

            editor.setupDockSpace (id, left, right, bottom);
            ImGui::DockBuilderFinish (id);
        }

        editorSettings.setTheme();
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu ("File"))
        {
            if (ImGui::MenuItem ("New", "Ctrl+N"))
            {
            }
            if (ImGui::MenuItem ("Save", "Ctrl+S"))
            {
            }
            if (ImGui::MenuItem ("Open", "Ctrl+O"))
            {
            }
            if (ImGui::MenuItem ("Settings"))
            {
                editorSettings.toggleOpen();
            }
            if (ImGui::MenuItem ("Quit", "Alt+F4"))
            {
                running = false;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu ("Edit"))
        {
            if (ImGui::MenuItem ("Undo", "Ctrl+Z"))
            {
            }
            if (ImGui::MenuItem ("Redo", "Ctrl+Shift+Z"))
            {
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu ("Run"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu ("Window"))
        {
            if (ImGui::MenuItem ("Logger"))
            {
                showLogger = true;
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();

        if (showLogger)
        {
            Logger::getLogger().draw (&showLogger);
        }

        editor.present (context.getFrameContext());
        editorSettings.present();

        ImGUIHelpers::endFrame (context.getFrameContext());

        context.endFrame();
    }
}