#include <LevelData/project.hpp>

void Project::loadFile (std::filesystem::path path)
{
}

void Project::saveFile (std::filesystem::path path)
{
}

void Project::to_json (nlohmann::json& j, const Project& p)
{
    j = nlohmann::json {
        { "Name", p.name }
    };
}
void Project::from_json (const nlohmann::json& j, Project& p)
{
    j.at ("Name").get_to (p.name);
}