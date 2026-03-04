#pragma once 
#include <map>

class Scene {
public:
    virtual void render()=0;
    virtual void update()=0;
};

class SceneManager final {

public:

    SceneManager();

    /// @brief Change the current scene to a new one, with a scene transition.
    /// @param sceneId The ID of the new  Scene
    /// @param playSceneTransition Should the transition animation play.
    void changeScene(int sceneId, bool playSceneTransition = true);

    /// @brief Renders the current scene to the screen.
    void renderScene();

    /// @brief Runs the update logic of the current scene
    void updateScene();

    /// @brief Draws debug information for Scene manager.
    void debugView();


private:
       Scene* currentScene;
};