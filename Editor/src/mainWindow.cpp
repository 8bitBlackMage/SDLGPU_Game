#include "mainWindow.hpp"
#include "Imgui/imguiHelpers.hpp"
#include "SDL3/SDL_dialog.h"
#include "Utils/fileHelpers.hpp"
#include "imgui_internal.h"
#include <Utils/logger.hpp>
#include <cstddef>

#ifdef __APPLE__
constexpr const char* saveShortcut = "Ctrl+S";
constexpr const char* newShortcut = "Ctrl+n";
constexpr const char* openShortcut = "Ctrl+n";
#else
constexpr const char* saveShortcut = "Ctrl+s";
constexpr const char* newShortcut = "Ctrl+n";
constexpr const char* openShortcut = "Ctrl+n";
#endif

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
    SDL_DialogFileFilter projectFiles {
        .name = "Concorde Projects",
        .pattern = "ccproj",
    };

    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu ("File"))
    {
        if (ImGui::MenuItem ("New", newShortcut))
        {
        }
        if (ImGui::MenuItem ("Save", saveShortcut))
        {
            SDL_ShowSaveFileDialog ([] (void* userdata, const char* const* filelist, int filter) {

            },
                                    nullptr,
                                    graphicsContext.getWindow(),
                                    &projectFiles,
                                    1,
                                    getUserHomePath().append ("Documents/").c_str());
        }
        if (ImGui::MenuItem ("Open", openShortcut))
        {
            SDL_ShowOpenFileDialog (
                [] (void* userdata, const char* const* filelist, int filter)
                {
                    Logger::log (filelist[0]);
                },
                nullptr,
                graphicsContext.getWindow(),
                &projectFiles,
                1,
                getUserHomePath().append ("Documents/").c_str(),
                false);
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