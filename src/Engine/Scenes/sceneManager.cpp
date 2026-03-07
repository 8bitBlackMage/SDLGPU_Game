#include "sceneManager.hpp"
#include "testScene.hpp"
#include <imgui.h>

SceneManager::SceneManager() : currentSceneName (emptySceneName)
{
    factories["test_scene"] = []
    {
        return std::make_unique<TestScene>();
    };
}

void SceneManager::changeScene (const std::string& sceneName, bool playSceneTransition)
{
}

void SceneManager::renderScene()
{
    if (currentScene)
    {
        currentScene->render();
    }
}

void SceneManager::updateScene()
{
    if (currentScene)
    {
        currentScene->update();
    }
}

std::unique_ptr<Scene> SceneManager::createScene (const std::string& sceneName)
{
    return factories[sceneName]();
}

void SceneManager::debugView()
{
    ImGui::Begin ("Scene Manager");
    ImGui::Text ("Current Scene : %s", currentSceneName.c_str());
    for (auto& sceneType : factories)
    {
        ImGui::Button ("Enable");
        ImGui::SameLine();
        ImGui::Text ("%s", sceneType.first.c_str());
    }
    if (ImGui::Button ("Close Scene"))
    {
        currentScene = nullptr;
    }
    ImGui::End();
}
