#pragma once 
#include <imgui.h>
#include <sstream> 

#include <iostream>
#include <iomanip>
#include <ctime>

class Logger
{
public: 
    static Logger&  getLogger();

    template< typename ... Args >
    static void log(Args const& ... args){
        Logger::getLogger().appendToLog(args...);
    }
    void draw();

    template< typename ... Args >
    void appendToLog(Args const& ... args)
    {
    int oldSize = buffer.size();
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream stream;
    stream << std::put_time(&tm, "%H:%M:%S") << " ";

    using List= int[];
    (void)List{0,( (void)(stream << args << " "),0 ) ...};

    stream << std::endl;

    auto str = stream.str();

    std::cout << str;

    buffer.append(str.c_str());

    for(int newSize = buffer.size(); oldSize< newSize; oldSize++)
    {
        if(buffer[oldSize] == '\n')
        {
            lineOffsets.push_back(oldSize);
        }
    }
    scrollToBottom = true;
}

    Logger(Logger const&) = delete;
    void operator=(Logger const&) = delete;
private:
    Logger();

    ImGuiTextBuffer buffer;
    ImGuiTextFilter filter;
    ImVector<int>   lineOffsets;
    bool scrollToBottom;

    void clear();


};