#pragma once
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>

class Project
{
public:
    Project() = default;
    std::string name;
    std::map<uint, std::string> scenes;
    std::map<uint, std::string> entityDefs;
    std::map<uint, std::string> assets;

    bool saveFile (std::filesystem::path filePath);
    bool loadFile (std::filesystem::path filePath);

    void to_json (nlohmann::json& j, const Project& p);
    void from_json (const nlohmann::json& j, Project& p);

private:
    std::filesystem::path projectPath;
};

static Project* newProject (std::string name, std::filesystem::path path);
