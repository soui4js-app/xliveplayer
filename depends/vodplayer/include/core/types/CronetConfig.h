#pragma once

#include <string>
#include <vector>

namespace transvod {

struct CronetConfig {
    struct QuicHint final {
        std::string host;
        int port;
        int altPort;
    };
    std::string experimentalOptions;
    std::vector<QuicHint> quicHints;
};
} // namespace transvod
