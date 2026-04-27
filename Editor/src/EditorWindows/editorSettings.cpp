#include "editorSettings.hpp"
#include <Imgui/imguiTheme.hpp>
#include <imgui.h>

void EditorSettings::toggleOpen()
{
    open = ! open;
}

void EditorSettings::setTheme()
{
    switch (currentTheme)
    {
        case 0:
        {
            ImGUIHelpers::styleColourDark();
            break;
        }
        case 1:
        {
            ImGUIHelpers::styleColourLight();
            break;
        }
        default:
        {
        }
    }
}

void EditorSettings::present()
{
    if (open)
    {
        ImGui::SetNextWindowSizeConstraints ({ 500, 500 }, { 1920, 1080 });
        ImGui::Begin (" Settings", &open, ImGuiWindowFlags_NoDocking);
        std::string& themePreview = themes[currentTheme];
        if (ImGui::BeginCombo ("Theme", themePreview.c_str()))
        {
            for (int i = 0; i < 2; i++)
            {
                const bool is_selected = (currentTheme == i);
                if (ImGui::Selectable (themes[i].c_str(), is_selected))
                {
                    currentTheme = i;
                }
                if (is_selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        ImGui::End();
    }
}