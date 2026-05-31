#include <Utils/ApplicationDefs.hpp>
#include <string>

#ifndef APPLICATION_NAME
#define APPLICATION_NAME "Olympus"
#endif
#ifndef APPLICATION_VERSION
#define APPLICATION_VERSION "1.0"
#endif
#ifndef APPLICATION_DESCRIPTION
#define APPLICATION_DESCRIPTION "Olympus Engine"
#endif

#ifdef __aarch64__
std::string ApplicationDefs::applicationArch ("ARM64");
#elifdef __amd64__ || __X86_64__ || _M_AMD64
std::string ApplicationDefs::applicationArch ("AMD64");
#elifdef __i386__ || _M_I86
std::string ApplicationDefs::applicationArch ("X86");
#elifdef __powerpc__ || __powerpc64__
std::string ApplicationDefs::applicationArch ("POWER PC");
#else
std::string ApplApplicationDefs::applicationArch ("Unknown (email me pls)")
#endif

#ifdef __linux__
std::string ApplicationDefs::applicationOS ("GNU/Linux");
#elifdef __APPLE__
std::string ApplicationDefs::applicationOS ("MacOS");
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
std::string ApplicApplicationDefs::applicationOS ("Windows");
#else
std::string AApplicationDefs::applicationOS ("Unknown (email me pls)");
#endif

std::string ApplicationDefs::applicationName (APPLICATION_NAME);
std::string ApplicationDefs::applicationVersion (APPLICATION_VERSION);
std::string ApplicationDefs::applicationDesc (APPLICATION_DESCRIPTION);
