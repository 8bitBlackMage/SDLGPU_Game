#ifndef CONCORDE_PROJECTSETTINGS_H
#define CONCORDE_PROJECTSETTINGS_H
#include "LevelData/project.hpp"
#include "imgui.h"

class ProjectSettings
{
public:
    void present (Project* currentProject);
    void setupDockSpace (ImGuiID centre, ImGuiID left, ImGuiID right, ImGuiID bottom);

    void newScenePopUp();

    void newEntityDefinitionPopUp();

private:
    bool showNewScene = false;
    bool showNewEntityDef = false;
};

#endif //CONCORDE_PROJECTSETTINGS_H