#pragma once

#include <filesystem>

void setApplicationRootPath (const char* pathstr);

std::filesystem::path getApplicationRootPath();

std::filesystem::path getAssetFolderPath();

std::filesystem::path getUserHomePath();

std::filesystem::path getSettingsFolderPath();