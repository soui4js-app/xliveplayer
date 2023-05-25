#pragma once

namespace transvod {
enum NetState {
    SYSNET_WIFI = 0,
    SYSNET_MOBILE = 1,
    SYSNET_DISCONNECT = 2,
    SYSNET_2G = 3,
    SYSNET_3G = 4,
    SYSNET_4G = 5,
    SYSNET_WLAN = 6,
    SYSNET_5G = 7,
    SYSNET_Ethernet = 8,
    SYSNET_UNKNOWN = 127
};
}
