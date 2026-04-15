#include "mainWindow.hpp"
#include "Imgui/imguiHelpers.hpp"
#include "imgui_internal.h"
#include <Utils/logger.hpp>
#include <cstddef>

MainWindow::MainWindow()
{
    graphicsContext.initContext ("Concorde");

    sceneEditor.init (nullptr, &graphicsContext);

    ImGUIHelpers::initContext (&graphicsContext);
}
void MainWindow::run()
{
    running = true;
    loop();
}

void MainWindow::loop()
{
    while (running)
    {
        graphicsContext.startFrame();
        auto id = ImGUIHelpers::startFrame();
        if (firstFrame)
        {
            setupDockSpace (id);
            firstFrame = false;
        }

        present();
        handleEvents();

        ImGUIHelpers::endFrame (graphicsContext.getFrameContext());
        graphicsContext.endFrame();
    }
}

void MainWindow::present()
{
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

    sceneEditor.present (graphicsContext.getFrameContext());
    editorSettings.present();
}
void MainWindow::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent (&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            running = false;
        }
        ImGUIHelpers::handleEvents (&event);
    }
}

void MainWindow::setupDockSpace (ImGuiID id)
{
    const ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoWindowMenuButton;
    ImGui::DockBuilderRemoveNode (id);
    ImGui::DockBuilderAddNode (id, dockspaceFlags);
    auto left = ImGui::DockBuilderSplitNode (id, ImGuiDir_Left, 0.2f, nullptr, &id);
    auto right = ImGui::DockBuilderSplitNode (id, ImGuiDir_Right, 0.2f, nullptr, &id);
    auto bottom = ImGui::DockBuilderSplitNode (id, ImGuiDir_Down, 0.3f, nullptr, &id);

    ImGui::DockBuilderDockWindow ("Log", bottom);

    sceneEditor.setupDockSpace (id, left, right, bottom);
    ImGui::DockBuilderFinish (id);
}