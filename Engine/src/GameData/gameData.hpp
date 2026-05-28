//
// Created by Alice Exley on 15/05/2026.
//

#ifndef DUNGEONCRAWLER_GAMEDATA_HPP
#define DUNGEONCRAWLER_GAMEDATA_HPP
#include <filesystem>
#include <vector>

class Scene;
class EntityDefinition;

///@brief container class for entire Game Data.
class GameData
{
public:
    GameData() = default;

    //file handling functions.
    static GameData* createNewProject (std::filesystem::path path);
    static GameData* loadFromProjectFile (std::filesystem::path path);
    static GameData* loadFromCompressedBinary (std::filesystem::path path);
    std::string* getName() { return &gameName; }
    std::vector<Scene*>& getScenes() { return scenes; }

    void serialize (std::filesystem::path path);
    void deserialize (std::filesystem::path path);

private:
    std::string gameName = "Untitled Game";
    std::vector<Scene*> scenes;
    std::vector<EntityDefinition*> entityDefs;
};

#endif //DUNGEONCRAWLER_GAMEDATA_HPP
