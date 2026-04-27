#ifndef CONCORDE_MAINWINDOW_H
#define CONCORDE_MAINWINDOW_H

#include "EditorWindows/editorSettings.hpp"

#include "EditorWindows/projectSettings.hpp"
#include "EditorWindows/sceneEditor.hpp"
#include "Graphics/graphicsContext.hpp"

#include "LevelData/project.hpp"
#include "imgui.h"
class MainWindow
{
public:
    MainWindow();
    void run();

    void loop();

    void present();
    void handleEvents();

    void setupDockSpace (ImGuiID id);

private:
    Project currentProject;

    bool firstFrame = true;
    bool running = true;

    bool showLogger = false;

    GraphicsContext graphicsContext;


    ProjectSettings projectSettings;
    SceneEditor sceneEditor;
    EditorSettings editorSettings;
};

#endif //CONCORDE_MAINWINDOW_H