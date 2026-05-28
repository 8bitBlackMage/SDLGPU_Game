//
// Created by Alice Exley on 15/05/2026.
//

#ifndef DUNGEONCRAWLER_SCENEDATA_HPP
#define DUNGEONCRAWLER_SCENEDATA_HPP
#include <string>

class Scene
{
public:
    const std::string* getName() { return &name; }

private:
    std::string name;
};

#endif //DUNGEONCRAWLER_SCENEDATA_HPP
