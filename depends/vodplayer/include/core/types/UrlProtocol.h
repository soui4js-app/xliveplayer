#pragma once

namespace transvod {

enum UrlProtocol {
    UrlProtocol_Http = 0,
    UrlProtocol_Quic = 1,
    UrlProtocol_File = 2,
    UrlProtocol_p2p = 4,

    UrlProtocol_User = 100, // 用户自定义自己的协议
};

enum RtShareInnerProtocol {
    RtShareInnerProtocol_Http = 0,
    RtShareInnerProtocol_P2p = 1,
};

} // namespace transvod
