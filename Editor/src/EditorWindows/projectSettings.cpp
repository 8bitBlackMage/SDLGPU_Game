
#include "projectSettings.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"
#include "windowIDs.hpp"
#include <string>

void ProjectSettings::present (Project* currentProject)
{
    ImGui::Begin (WindowIDs::projectSettings);
    ImGui::InputText ("Project Name ", &currentProject->name);
    if (ImGui::TreeNodeEx ("Scenes", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImGui::BeginTable ("Scenes", 1, ImGuiTableFlags_RowBg);

        for (auto& scene : currentProject->scenes)
        {
            ImGui::TableNextRow();
            bool isSelected;
            ImGui::TableSetColumnIndex (0);
            ImGui::PushStyleVar (ImGuiStyleVar_ItemSpacing, ImVec2 (style.ItemSpacing.x, style.CellPadding.y * 2)); // Fix
            ImGui::Selectable (scene.second.c_str(), isSelected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap, { 0, 0 });
            ImGui::PopStyleVar();
        }
        ImGui::EndTable();

        ImGui::TreePop();
    }
    ImGui::Separator();
    if (ImGui::Button ("󰦃 New Scene"))
    {
        ImGui::OpenPopup ("New Scene");
    }
    if (ImGui::TreeNodeEx ("Entity Definitions", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImGui::BeginTable ("Entity Definitions", 1, ImGuiTableFlags_RowBg);

        for (auto& entityDef : currentProject->entityDefs)
        {
            ImGui::TableNextRow();
            bool isSelected;
            ImGui::TableSetColumnIndex (0);
            ImGui::PushStyleVar (ImGuiStyleVar_ItemSpacing, ImVec2 (style.ItemSpacing.x, style.CellPadding.y * 2)); // Fix
            ImGui::Selectable (entityDef.second.c_str(), isSelected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap, { 0, 0 });
            ImGui::PopStyleVar();
        }
        ImGui::EndTable();

        ImGui::TreePop();
    }
    ImGui::Separator();
    if (ImGui::Button ("󰙑 New Entity Definition"))
    {
        ImGui::OpenPopup ("New Entity Definition");
    }
    newEntityDefinitionPopUp();

    newScenePopUp();

    ImGui::End();
}

void ProjectSettings::newScenePopUp()
{
    std::string filepath;
    ImGui::SetNextWindowSizeConstraints ({ 500, 200 }, { 500, 200 });
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos (center, ImGuiCond_Appearing, ImVec2 (0.5f, 0.5f));
    if (ImGui::BeginPopupModal ("New Scene", nullptr,ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoScrollWithMouse |ImGuiWindowFlags_NoResize))
    {
        ImGui::Dummy(ImVec2(0.0f, 25.0f));
        ImGui::Text ("File Path");
        ImGui::InputText ("##FilePath",&filepath);
        ImGui::TextColored ({0.24705882f,0.74901961f ,0.24705882f,1.0f},"Path is Valid");
        ImGui::Dummy(ImVec2(0.0f, 25.0f));
        if (ImGui::Button ("Ok"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button ("Close"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void ProjectSettings::newEntityDefinitionPopUp()
{
    std::string filepath;
    ImGui::SetNextWindowSizeConstraints ({ 500, 200 }, { 500, 200 });
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos (center, ImGuiCond_Appearing, ImVec2 (0.5f, 0.5f));
    if (ImGui::BeginPopupModal ("New Entity Definition", nullptr,ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoScrollWithMouse |ImGuiWindowFlags_NoResize))
    {
        ImGui::Dummy(ImVec2(0.0f, 25.0f));
        ImGui::Text ("File Path");
        ImGui::InputText ("##FilePath",&filepath);
        ImGui::TextColored ({0.24705882f,0.74901961f ,0.24705882f,1.0f},"Path is Valid");
        ImGui::Dummy(ImVec2(0.0f, 25.0f));
        if (ImGui::Button ("Ok"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button ("Close"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void ProjectSettings::setupDockSpace (ImGuiID centre, ImGuiID left, ImGuiID right, ImGuiID bottom)
{
    ImGui::DockBuilderDockWindow ("Project Settings", left);
}
