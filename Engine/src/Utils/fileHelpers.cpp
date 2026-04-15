#include <Utils/fileHelpers.hpp>
#include <filesystem>
#if defined(__APPLE__)
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

static std::filesystem::path appRootPath;

void setApplicationRootPath (const char* pathstr)
{
    appRootPath = pathstr;
}

std::filesystem::path getApplicationRootPath()
{
    return appRootPath;
}

std::filesystem::path getAssetFolderPath()
{
    return getApplicationRootPath().parent_path().append ("assets");
}

std::filesystem::path getUserHomePath()
{
    struct passwd* pw = getpwuid (getuid());

    return std::filesystem::path (pw->pw_dir);
}

std::filesystem::path getSettingsFolderPath()
{
    return getUserHomePath().append ("Library/Preferences/");
}
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
std::filesystem::path getSettingsFolderPath()
{
}
#endif