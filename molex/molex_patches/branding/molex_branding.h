#ifndef MOLEX_BRANDING_MOLEX_BRANDING_H_
#define MOLEX_BRANDING_MOLEX_BRANDING_H_

#include <string>

#include "build/build_config.h"

namespace molex {

constexpr char kProductName[] = "Molex";
constexpr char kProductShortName[] = "Molex";
constexpr char kProductUrl[] = "https://molex-browser.dev";
#ifndef MOLEX_VERSION
#define MOLEX_VERSION "1.0.0.0"
#endif
constexpr char kProductVersion[] = MOLEX_VERSION;

#if BUILDFLAG(IS_WIN)
constexpr char kAppGuid[] = "{MOLEX-APP-GUID-0000-0000-000000000000}";
#else
constexpr char kAppGuid[] = "";
#endif

constexpr char kCrashServerUrl[] = "";

constexpr char kUpdateServerUrl[] = "https://updates.molex-browser.dev/v1/update";

constexpr char kWidevineUrl[] = "https://updates.molex-browser.dev/widevine";

}

#endif
