//
// Created by Alice Exley on 15/05/2026.
//

#include "gameData.hpp"

#include "scene.hpp"

#include <Utils/logger.hpp>
#include <fstream>
#include <nlohmann/json.hpp>

GameData* GameData::createNewProject (std::filesystem::path path)
{
    if (is_directory (path))
    {
        Logger::log ("Directory with name already exists, cannot create project");
        return nullptr;
    }

    if (! std::filesystem::create_directory (path))
    {
        return nullptr;
    }

    auto projectFile = path.append ("project.json");
    auto data = new GameData;
    data->serialize (projectFile);
    return data;
}
GameData* GameData::loadFromProjectFile (std::filesystem::path path)
{
    return nullptr;
}
GameData* GameData::loadFromCompressedBinary (std::filesystem::path path)
{
    return nullptr;
}
void GameData::serialize (std::filesystem::path path)
{
    nlohmann::ordered_json jsonData;
    jsonData["Name"] = *getName();

    nlohmann::json sceneArray = nlohmann::json::array();
    for (auto scene : scenes)
    {
        auto sceneObject = nlohmann::ordered_json {};
        sceneObject["Name"] = *scene->getName();
        sceneObject["ID"] = 0;
        sceneObject["Path"] = "/";
        sceneArray.push_back ({ sceneObject });
    }
    jsonData["Scenes"] = sceneArray;

    nlohmann::json entityDefArray = nlohmann::json::array();
    jsonData["EntityDefinitions"] = entityDefArray;

    std::ofstream f (path.string());

    f << jsonData.dump (4);
    Logger::log (jsonData.dump (4));
    f.close();
}
void GameData::deserialize (std::filesystem::path path)
{
}