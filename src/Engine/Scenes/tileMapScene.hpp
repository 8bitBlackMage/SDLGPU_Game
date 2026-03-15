#pragma once
#include "LDtkLoader/World.hpp"
#include <Engine/Scenes/scene.hpp>

#include <LDtkLoader/Project.hpp>

class TileMapScene final : public Scene
{
public:
    TileMapScene();

    void render() override;
    void update() override;

private:
    ldtk::Project project;
};