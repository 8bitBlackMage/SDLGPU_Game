#pragma once
#include "LevelData/Maps/layer.hpp"
#include <nlohmann/json.hpp>
#include <vector>

class TileLayer : public Layer
{
public:
    TileLayer() = default;
    int getTileAtPosition (int x, int y);
    void setTilePosition (int tileId, int x, int y);

    NLOHMANN_DEFINE_TYPE_INTRUSIVE (TileLayer, width, height, gridSize, tiles);

private:
    int width, height;
    int gridSize;
    std::vector<int> tiles;
};