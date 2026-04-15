#pragma once
#include <nlohmann/json.hpp>
#include <string>
struct Project
{
public:
    Project() = default;
    std::string name;

    void saveFile (std::filesystem::path filePath);
    void loadFile (std::filesystem::path filePath);

    void to_json (nlohmann::json& j, const Project& p);
    void from_json (const nlohmann::json& j, Project& p);
};
