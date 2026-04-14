
#include "SDL3/SDL_events.h"
#include "tileMapEditor.hpp"
#include <Graphics/graphicsContext.hpp>
#include <Imgui/imguiHelpers.hpp>
#include <imgui.h>

int main()
{
    Scene scene;
    scene.width = 320;
    scene.height = 240;
    scene.gridSize = 32;

    bool running = true;
    GraphicsContext context;
    TileMapEditor editor;

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
        ImGUIHelpers::startFrame();
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

        ImGui::EndMainMenuBar();

        ImGui::Begin ("Scene");
        ImGui::End();

        ImGui::Begin ("Inspector");

        ImGui::End();

        ImGui::Begin ("Log");

        ImGui::End();

        ImGui::Begin ("Edit Tools");

        ImGui::End();

        editor.render (context.getFrameContext());

        ImGUIHelpers::endFrame (context.getFrameContext());

        context.endFrame();
    }
}