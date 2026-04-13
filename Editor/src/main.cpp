
#include "SDL3/SDL_events.h"
#include "tileMapEditor.hpp"
#include <Graphics/graphicsContext.hpp>
#include <Imgui/imguiHelpers.hpp>
#include <imgui.h>
int main()
{
    bool running = true;
    GraphicsContext context;
    TileMapEditor editor;

    context.initContext();
    editor.init (&context);
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
            if (ImGui::MenuItem ("New"))
            {
            }
            if (ImGui::MenuItem ("Save"))
            {
            }
            if (ImGui::MenuItem ("Open"))
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