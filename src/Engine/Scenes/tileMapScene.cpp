#include "LDtkLoader/Project.hpp"
#include <Engine/Scenes/tileMapScene.hpp>

TileMapScene::TileMapScene()
{
    project.loadFromFile ("assets/levels/GdungeonCrawler.ldtk");
}

void TileMapScene::render()
{
}

void TileMapScene::update()
{
}