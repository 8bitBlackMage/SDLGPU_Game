#pragma once

#include "LevelData/Maps/layer.hpp"
#include <memory>
#include <vector>
class Scene
{
public:
    std::vector<std::unique_ptr<Layer>> layers;
    int width;
    int height;
    int gridSize;
};