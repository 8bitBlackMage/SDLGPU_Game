#pragma once

#include "LevelData/Maps/layer.hpp"
#include <vector>

class TileLayer : public Layer
{
public:
    int getTileAtPosition (int x, int y);
    void setTilePosition (int tileId, int x, int y);

private:
    int gridSize;
    std::vector<int> tiles;
};