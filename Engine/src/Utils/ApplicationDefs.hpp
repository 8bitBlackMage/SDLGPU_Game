#pragma once

//This Populates a range of variables from CMake assets
//(this may change based on project definitions via project builds)
//
#include <string>

namespace ApplicationDefs
{
extern std::string applicationName;
extern std::string applicationVersion;
extern std::string applicationDesc;
extern std::string applicationArch;
extern std::string applicationOS;
} // namespace ApplicationDefs
