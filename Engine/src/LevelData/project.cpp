#include "Utils/logger.hpp"
#include <LevelData/project.hpp>
#include <exception>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

Project newProject (std::filesystem::path path)
{
    try
    {
        std::filesystem::create_directory (path.parent_path());
    }
    catch (std::filesystem::filesystem_error& error)
    {
        Logger::log (error.code(), error.what(), error.path1());
    }

    Project proj;

    if (! proj.saveFile (path))
    {
    }

    auto scenePath = path.parent_path().append ("Scenes");
    auto entityDefPath = path.parent_path().append ("EntityDefs");
    auto assetPath = path.parent_path().append ("Assets");

    try
    {
        std::filesystem::create_directory (scenePath);
    }
    catch (std::filesystem::filesystem_error& error)
    {
        Logger::log (error.code(), error.what(), error.path1());
    }
    try
    {
        std::filesystem::create_directory (entityDefPath);
    }
    catch (std::filesystem::filesystem_error& error)
    {
        Logger::log (error.code(), error.what(), error.path1());
    }
    try
    {
        std::filesystem::create_directory (assetPath);
    }
    catch (std::filesystem::filesystem_error& error)
    {
        Logger::log (error.code(), error.what(), error.path1());
    }

    return proj;
}

bool Project::loadFile (std::filesystem::path path)
{
    std::ifstream ifstream (path.string());
    auto j = nlohmann::json::parse (ifstream);
    from_json (j, *this);

    ifstream.close();
    return true;
}

bool Project::saveFile (std::filesystem::path path)
{
    std::ofstream ofstream (path.string());
    nlohmann::json j;
    to_json (j, *this);

    ofstream << j;

    ofstream.close();

    return true;
}

void Project::to_json (nlohmann::json& j, const Project& p)
{
    j = nlohmann::json {
        { "Name", p.name },
        { "Scenes", p.scenes },
        { "EntityDefs", p.entityDefs },
        { "Assets", p.assets },

    };
}
void Project::from_json (const nlohmann::json& j, Project& p)
{
    name = j["Name"].get<std::string>();
    scenes = j["Scenes"].get<std::map<uint, std::string>>();
}