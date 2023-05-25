#pragma once

#include <string>

namespace transvod {

enum PlayMode {
  PlayMode_Vod = 1,
  PlayMode_Live = 2,
};

struct TransInitConfig final {
    std::string sdkVer;
    std::string sysModel;
    std::string sysVer;
    uint32_t playerContextId;
    std::string cacheDir;
};
} // namespace transvod
