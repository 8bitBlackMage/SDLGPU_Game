#pragma once

class AboutWindow
{
public:
    void toggleOpen();

    void present();

private:
    bool open = false;
};
