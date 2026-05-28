#ifndef CONCORDE_PROJECTSETTINGS_H
#define CONCORDE_PROJECTSETTINGS_H
#include "GameData/gameData.hpp"
#include "imgui.h"

class ProjectSettings
{
public:
    void present (GameData* currentProject);
    void setupDockSpace (ImGuiID centre, ImGuiID left, ImGuiID right, ImGuiID bottom);

    void newScenePopUp();

    void newEntityDefinitionPopUp();

private:
    bool showNewScene = false;
    bool showNewEntityDef = false;
};

#endif //CONCORDE_PROJECTSETTINGS_H