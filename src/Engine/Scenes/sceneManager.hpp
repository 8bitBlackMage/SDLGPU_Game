#pragma once
#include <map>
#include <memory>

#include "scene.hpp"
#include <functional>

/// @brief This object handles the lifetime and operation of scenes, along with displaying debugging info.
class SceneManager final
{
public:
    SceneManager();

    /// @brief Change the current scene to a new one, with a scene transition.
    /// @param sceneName The name of the new  Scene
    /// @param playSceneTransition Should the transition animation play.
    void changeScene (const std::string& sceneName, bool playSceneTransition = true);

    /// @brief Renders the current scene to the screen.
    void renderScene();

    /// @brief Runs the update logic of the current scene
    void updateScene();

    /// @brief Draws debug information for Scene manager.
    void debugView();

private:
    std::unique_ptr<Scene> createScene (const std::string& sceneName);
    const std::string& currentSceneName;

    const std::string emptySceneName = "None";
    std::map<std::string, std::function<std::unique_ptr<Scene>()>> factories;
    std::unique_ptr<Scene> currentScene;
};