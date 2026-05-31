#include "Utils/fileHelpers.hpp"
#include <game.hpp>

int main (int argc, const char** argv)
{
    setApplicationRootPath (argv[0]);
    Game game;

    game.run();
}
