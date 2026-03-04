#include "sceneManager.hpp"

#include <imgui.h>

SceneManager::SceneManager()
{
}

void SceneManager::changeScene(int sceneId, bool playSceneTransition)
{
}

void SceneManager::renderScene()
{
}

void SceneManager::updateScene()
{
}

void SceneManager::debugView()
{
    ImGui::Begin("Scene Manager");
    ImGui::Text("test");

    ImGui::End();
}
