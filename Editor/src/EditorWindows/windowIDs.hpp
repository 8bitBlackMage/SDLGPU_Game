//
// Created by Alice Exley on 23/04/2026.
//

#ifndef CONCORDE_WINDOWIDS_H
#define CONCORDE_WINDOWIDS_H




namespace WindowIDs
{
class WindowID
{
public:
    WindowID(const char* text): data(text){}
    operator const char *() {return data.c_str(); }
private:
    std::string data;
};


static WindowID editorSettings = "Settings";
static WindowID projectSettings = "Project Settings";
static WindowID sceneSettings = "󰨁 Scene Settings";
static WindowID sceneEditor = "󰍍 Scene Editor";
static WindowID fileBrowser = " File Browser";
}
#endif // CONCORDE_WINDOWIDS_H
