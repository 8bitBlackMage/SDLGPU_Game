#pragma once

/// @brief pure virtual parent class for Scenes. Inherit to create a scene that can be handled by a scene manager.
class Scene {
public:
    /// @brief pure virtual render function. must be overriden. all drawing code goes in here.
    virtual void render()=0;
    /// @brief pure virtual update function. all logic goes in here. 
    virtual void update()=0;
};