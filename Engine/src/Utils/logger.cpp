#include <Utils/logger.hpp>

Logger::Logger() {}

Logger& Logger::getLogger()
{
    static Logger logger;

    return logger;
}

void Logger::draw (bool* close)
{
    ImGui::Begin ("Log", close);
    if (ImGui::Button ("Clear"))
    {
        clear();
    }
    ImGui::SameLine();
    bool copy = ImGui::Button ("Copy");
    ImGui::SameLine();
    filter.Draw ("Filter", -100.f);
    ImGui::BeginChild ("Scrolling");
    ImGui::PushStyleVar (ImGuiStyleVar_ItemSpacing, ImVec2 (0, 1));

    if (copy)
    {
        ImGui::LogToClipboard();
    }

    if (filter.IsActive())
    {
        const char* bufStart = buffer.begin();
        const char* line = bufStart;
        for (int lineNumber = 0; line != nullptr; lineNumber++)
        {
            const char* lineEnd = (lineNumber < lineOffsets.Size) ? bufStart + lineOffsets[lineNumber] : nullptr;
            if (filter.PassFilter (line, lineEnd))
            {
                ImGui::TextUnformatted (line, lineEnd);
            }
            line = lineEnd && lineEnd[1] ? lineEnd + 1 : nullptr;
        }
    }
    else
    {
        ImGui::TextUnformatted (buffer.begin());
    }
    if (scrollToBottom)
    {
        ImGui::SetScrollHereY (1.0f);
    }
    scrollToBottom = false;
    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::End();
}

void Logger::clear()
{
}