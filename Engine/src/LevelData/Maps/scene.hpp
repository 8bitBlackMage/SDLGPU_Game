#pragma once

#include "LevelData/Maps/layer.hpp"

#include <memory>
#include <nlohmann/json.hpp>
#include <vector>

class Scene
{
public:
    Scene() = default;

    std::vector<std::unique_ptr<Layer>> layers;
    size_t uuid;
    std::string sceneName;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE (Scene, sceneName, uuid, layers);
};
