#include "Utils/ApplicationDefs.hpp"
#include <Utils/fileHelpers.hpp>
#include <filesystem>

//Unix helpers
#if defined(__APPLE__) || defined(__linux__)
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

std::filesystem::path getUserHomePath()
{
    struct passwd* pw = getpwuid (getuid());

    return std::filesystem::path (pw->pw_dir);
}

//WIN32 helpers
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

//#TODO implement this when I have access to a Windows install.

#endif

#if defined(__linux__)

std::filesystem::path getAssetFolderPath()
{
    return getUserHomePath().append (".local/share/" + ApplicationDefs::applicationName + "/assets");
}

std::filesystem::path getSettingsFolderPath()
{
    return getUserHomePath().append (".config/" + ApplicationDefs::applicationName);
}
#elif defined(__APPLE__)

std::filesystem::helpers getAssetFolderPath()
{
}

std::filesystem::path getSettingsFolderPath()
{
    return getUserHomePath().append ("Library/Preferences/");
}
#endif
