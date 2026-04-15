#include "Utils/fileHelpers.hpp"
#include "mainWindow.hpp"
int main (int argc, const char** argv)
{
    setApplicationRootPath (argv[0]);

    MainWindow window;

    window.run();
}