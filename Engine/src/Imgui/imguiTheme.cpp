
#include <Imgui/imguiTheme.hpp>
#include <imgui.h>

constexpr ImVec4 u8tofloatColour (unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    return {
        (float) r / 255.f,
        (float) g / 255.f,
        (float) b / 255.f,
        (float) a / 255.f,
    };
}

void ImGUIHelpers::styleLayout()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.Alpha = 1.0f;
    style.DisabledAlpha = 1.0f;
    style.WindowPadding = ImVec2 (12.0f, 12.0f);
    style.WindowRounding = 11.5f;
    style.WindowBorderSize = 0.1f;
    style.WindowMinSize = ImVec2 (20.0f, 20.0f);
    style.WindowTitleAlign = ImVec2 (0.5f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Right;
    style.ChildRounding = 0.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupRounding = 0.0f;
    style.PopupBorderSize = 1.0f;
    style.FramePadding = ImVec2 (20.0f, 3.400000095367432f);
    style.FrameRounding = 11.89999961853027f;
    style.FrameBorderSize = 0.0f;
    style.ItemSpacing = ImVec2 (4.300000190734863f, 5.5f);
    style.ItemInnerSpacing = ImVec2 (7.099999904632568f, 1.799999952316284f);
    style.CellPadding = ImVec2 (12.10000038146973f, 9.199999809265137f);
    style.IndentSpacing = 0.0f;
    style.ColumnsMinSpacing = 4.900000095367432f;
    style.ScrollbarSize = 11.60000038146973f;
    style.ScrollbarRounding = 15.89999961853027f;
    style.GrabMinSize = 3.700000047683716f;
    style.GrabRounding = 20.0f;
    style.TabRounding = 0.0f;
    style.TabBorderSize = 0.0f;
    //style.TabMinWidthForCloseButton = 0.0f;
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2 (0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2 (0.0f, 0.0f);
}

void ImGUIHelpers::styleColourDark()
{
    constexpr auto darkBackground = u8tofloatColour (65, 65, 65, 255);
    constexpr auto midBackground = u8tofloatColour (80, 80, 80, 255);
    constexpr auto lightBackground = u8tofloatColour (100, 100, 100, 255);

    constexpr auto text = u8tofloatColour (255, 255, 255, 255);
    constexpr auto disabledText = u8tofloatColour (150, 150, 150, 255);

    constexpr auto darkHighlight = u8tofloatColour (90, 90, 96, 255);
    constexpr auto midHighlight = u8tofloatColour (100, 100, 106, 255);
    constexpr auto brightHighlight = u8tofloatColour (130, 130, 136, 255);

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = text;
    style.Colors[ImGuiCol_TextDisabled] = disabledText;

    style.Colors[ImGuiCol_WindowBg] = darkBackground;
    style.Colors[ImGuiCol_ChildBg] = darkBackground;
    style.Colors[ImGuiCol_PopupBg] = darkBackground;

    style.Colors[ImGuiCol_Border] = midBackground;
    style.Colors[ImGuiCol_BorderShadow] = darkBackground;

    style.Colors[ImGuiCol_FrameBgActive] = lightBackground;
    style.Colors[ImGuiCol_FrameBgHovered] = darkBackground;
    style.Colors[ImGuiCol_FrameBg] = midBackground;

    style.Colors[ImGuiCol_TitleBg] = darkBackground;
    style.Colors[ImGuiCol_TitleBgActive] = midBackground;
    style.Colors[ImGuiCol_TitleBgCollapsed] = darkHighlight;

    style.Colors[ImGuiCol_MenuBarBg] = darkBackground;

    style.Colors[ImGuiCol_ScrollbarBg] = darkBackground;
    style.Colors[ImGuiCol_ScrollbarGrab] = midHighlight;
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = darkHighlight;
    style.Colors[ImGuiCol_ScrollbarGrabActive] = brightHighlight;

    style.Colors[ImGuiCol_CheckMark] = darkHighlight;

    style.Colors[ImGuiCol_SliderGrab] = midHighlight;
    style.Colors[ImGuiCol_SliderGrabActive] = brightHighlight;

    style.Colors[ImGuiCol_Button] = midHighlight;
    style.Colors[ImGuiCol_ButtonHovered] = brightHighlight;
    style.Colors[ImGuiCol_ButtonActive] = darkHighlight;

    style.Colors[ImGuiCol_Header] = midHighlight;
    style.Colors[ImGuiCol_HeaderHovered] = brightHighlight;
    style.Colors[ImGuiCol_HeaderActive] = midHighlight;

    style.Colors[ImGuiCol_Separator] = darkHighlight;
    style.Colors[ImGuiCol_SeparatorHovered] = brightHighlight;
    style.Colors[ImGuiCol_SeparatorActive] = midHighlight;

    style.Colors[ImGuiCol_ResizeGrip] = lightBackground;
    style.Colors[ImGuiCol_ResizeGripHovered] = brightHighlight;
    style.Colors[ImGuiCol_ResizeGripActive] = text;

    style.Colors[ImGuiCol_Tab] = darkHighlight;
    style.Colors[ImGuiCol_TabHovered] = brightHighlight;
    style.Colors[ImGuiCol_TabActive] = midHighlight;

    style.Colors[ImGuiCol_TabDimmed] = midHighlight;
    style.Colors[ImGuiCol_TabDimmedSelected] = brightHighlight;

    style.Colors[ImGuiCol_TabUnfocused] = midBackground;
    style.Colors[ImGuiCol_TabUnfocusedActive] = lightBackground;

    style.Colors[ImGuiCol_PlotLines] = midBackground;
    style.Colors[ImGuiCol_PlotLinesHovered] = lightBackground;
    style.Colors[ImGuiCol_PlotHistogram] = text;
    style.Colors[ImGuiCol_PlotHistogramHovered] = disabledText;

    style.Colors[ImGuiCol_TableHeaderBg] = midBackground;
    style.Colors[ImGuiCol_TableBorderStrong] = midHighlight;
    style.Colors[ImGuiCol_TableBorderLight] = lightBackground;
    style.Colors[ImGuiCol_TableRowBgAlt] = darkBackground;

    style.Colors[ImGuiCol_TextSelectedBg] = lightBackground;
    style.Colors[ImGuiCol_DragDropTarget] = midHighlight;

    style.Colors[ImGuiCol_NavHighlight] = midHighlight;

    style.Colors[ImGuiCol_NavWindowingHighlight] = brightHighlight;
    style.Colors[ImGuiCol_NavWindowingDimBg] = midBackground;
    style.Colors[ImGuiCol_ModalWindowDimBg] = darkBackground;

    style.Colors[ImGuiCol_DockingEmptyBg] = darkBackground;
}

void ImGUIHelpers::styleColourLight()
{
    constexpr auto darkBackground = u8tofloatColour (178, 178, 178, 255);
    constexpr auto midBackground = u8tofloatColour (198, 198, 198, 255);
    constexpr auto lightBackground = u8tofloatColour (226, 226, 226, 255);

    constexpr auto text = u8tofloatColour (0, 0, 0, 255);
    constexpr auto disabledText = u8tofloatColour (25, 25, 25, 255);

    constexpr auto darkHighlight = u8tofloatColour (57, 129, 187, 255);
    constexpr auto midHighlight = u8tofloatColour (73, 137, 207, 255);
    constexpr auto brightHighlight = u8tofloatColour (133, 187, 227, 255);

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = text;
    style.Colors[ImGuiCol_TextDisabled] = disabledText;

    style.Colors[ImGuiCol_WindowBg] = darkBackground;
    style.Colors[ImGuiCol_ChildBg] = darkBackground;
    style.Colors[ImGuiCol_PopupBg] = darkBackground;

    style.Colors[ImGuiCol_Border] = midBackground;
    style.Colors[ImGuiCol_BorderShadow] = darkBackground;

    style.Colors[ImGuiCol_FrameBgActive] = lightBackground;
    style.Colors[ImGuiCol_FrameBgHovered] = darkBackground;
    style.Colors[ImGuiCol_FrameBg] = midBackground;

    style.Colors[ImGuiCol_TitleBg] = darkBackground;
    style.Colors[ImGuiCol_TitleBgActive] = midBackground;
    style.Colors[ImGuiCol_TitleBgCollapsed] = darkHighlight;

    style.Colors[ImGuiCol_MenuBarBg] = darkBackground;

    style.Colors[ImGuiCol_ScrollbarBg] = darkBackground;
    style.Colors[ImGuiCol_ScrollbarGrab] = midHighlight;
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = darkHighlight;
    style.Colors[ImGuiCol_ScrollbarGrabActive] = brightHighlight;

    style.Colors[ImGuiCol_CheckMark] = darkHighlight;

    style.Colors[ImGuiCol_SliderGrab] = midHighlight;
    style.Colors[ImGuiCol_SliderGrabActive] = brightHighlight;

    style.Colors[ImGuiCol_Button] = midHighlight;
    style.Colors[ImGuiCol_ButtonHovered] = brightHighlight;
    style.Colors[ImGuiCol_ButtonActive] = darkHighlight;

    style.Colors[ImGuiCol_Header] = darkHighlight;
    style.Colors[ImGuiCol_HeaderHovered] = brightHighlight;
    style.Colors[ImGuiCol_HeaderActive] = midHighlight;

    style.Colors[ImGuiCol_Separator] = darkHighlight;
    style.Colors[ImGuiCol_SeparatorHovered] = brightHighlight;
    style.Colors[ImGuiCol_SeparatorActive] = midHighlight;

    style.Colors[ImGuiCol_ResizeGrip] = lightBackground;
    style.Colors[ImGuiCol_ResizeGripHovered] = brightHighlight;
    style.Colors[ImGuiCol_ResizeGripActive] = text;

    style.Colors[ImGuiCol_Tab] = darkHighlight;
    style.Colors[ImGuiCol_TabHovered] = brightHighlight;
    style.Colors[ImGuiCol_TabActive] = midHighlight;

    style.Colors[ImGuiCol_TabUnfocused] = midBackground;
    style.Colors[ImGuiCol_TabUnfocusedActive] = lightBackground;

    style.Colors[ImGuiCol_PlotLines] = midBackground;
    style.Colors[ImGuiCol_PlotLinesHovered] = lightBackground;
    style.Colors[ImGuiCol_PlotHistogram] = text;
    style.Colors[ImGuiCol_PlotHistogramHovered] = disabledText;

    style.Colors[ImGuiCol_TableHeaderBg] = midBackground;
    style.Colors[ImGuiCol_TableBorderStrong] = midHighlight;
    style.Colors[ImGuiCol_TableBorderLight] = lightBackground;
    style.Colors[ImGuiCol_TableRowBgAlt] = darkBackground;

    style.Colors[ImGuiCol_TextSelectedBg] = lightBackground;
    style.Colors[ImGuiCol_DragDropTarget] = midHighlight;

    style.Colors[ImGuiCol_NavHighlight] = midHighlight;

    style.Colors[ImGuiCol_NavWindowingHighlight] = brightHighlight;
    style.Colors[ImGuiCol_NavWindowingDimBg] = midBackground;
    style.Colors[ImGuiCol_ModalWindowDimBg] = darkBackground;

    style.Colors[ImGuiCol_DockingEmptyBg] = darkBackground;
}