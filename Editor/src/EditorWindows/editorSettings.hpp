#ifndef CONCORDE_EDITORSETTINGS_H
#define CONCORDE_EDITORSETTINGS_H
#include <string>

class EditorSettings
{
public:
    void toggleOpen();
    void present();

    void setTheme();

private:
    bool open = false;
    int currentTheme = 0;
    std::string themes[2] = { "Dark", "Light" };
};

#endif //CONCORDE_EDITORSETTINGS_H