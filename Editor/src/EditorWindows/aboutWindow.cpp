#include "aboutWindow.hpp"
#include "Utils/ApplicationDefs.hpp"
#include <imgui.h>
void AboutWindow::toggleOpen()
{
    open = ! open;
}

void AboutWindow::present()
{
    if (open)
    {
        ImGui::SetNextWindowSizeConstraints ({ 500, 300 }, { 500, 300 });
        ImGui::Begin ("About", &open, ImGuiWindowFlags_NoDocking);
        ImGui::PushFont (nullptr, 60);
        ImGui::Text ("Concorde Editor");
        ImGui::PopFont();
        ImGui::Text ("Version: %s", ApplicationDefs::applicationVersion.c_str());
        ImGui::Text ("By 8bitblackmage");

        ImGui::Text ("Platform: %s", ApplicationDefs::applicationOS.c_str());
        ImGui::Text ("CPU Arch: %s", ApplicationDefs::applicationArch.c_str());

        ImGui::End();
    }
}
