#pragma once
namespace transvod{
enum  PlayerState{
        PLAYER_UNKNOW = 0,
        PLAYER_IDLE = 1,
        PLAYER_READY = 2,
        PLAYER_LOADING = 3,
        PLAYER_PLAYING = 4,
        PLAYER_PAUSED = 5,
        PLAYER_ENDED = 6,
        PLAYER_INVALID = 7,
    };

    //卡顿原因
    enum  ErrorReason{
      REASON_OK = 0, // 播放正常
      REASON_BOTH_NETWORK_BAD = 1, // 主播、观众网络状态差
      REASON_ANCHOR_NETWORK_BAD = 2, // 主播网络状态差
      REASON_AUDIENCE_NETWORK_BAD = 3, // 观众网络状态差
    };
}