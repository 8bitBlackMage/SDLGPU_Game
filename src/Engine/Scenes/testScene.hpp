#pragma once
#include <Engine/Scenes/scene.hpp>

class TestScene : public Scene
{
public:
    TestScene();
    void render() override;
    void update() override;
};