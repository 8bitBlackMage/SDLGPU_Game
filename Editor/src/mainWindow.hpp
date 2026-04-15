#pragma once

#include "Graphics/graphicsContext.hpp"
#include "LevelData/project.hpp"
#include "editorSettings.hpp"
#include "sceneEditor.hpp"
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
    SceneEditor sceneEditor;
    EditorSettings editorSettings;
};