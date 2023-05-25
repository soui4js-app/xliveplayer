//
// Created by Administrator on 2020/6/9.
//

#pragma once

#include "core/common/IntType.h"

namespace transvod
{

enum PlayReason{
  // 播放正常
  PLAYREASON_OK = 0,

//卡顿原因:  主播、观众网络状态差
  PLAYREASON_BOTH_NETWORK_BAD = 1,

// 主播网络状态差
  PLAYREASON_ANCHOR_NETWORK_BAD = 2,

// 观众网络状态差
  PLAYREASON_AUDIENCE_NETWORK_BAD = 3,
};


struct NetworkQuality
{
  enum
  {
    NQ_UNKNOWN = 0, // 质量未知
    NQ_EXCELLENT = 1, // 网络质量极好
    NQ_GOOD = 2, // 网络质量好
    NQ_POOR = 3, // 网络质量较好，用户感受有瑕疵但不影响沟通
    NQ_BAD = 4, // 网络质量一般，勉强能沟通但不顺畅
    NQ_VBAD = 5, // 网络质量非常差，基本不能沟通
    NQ_DOWN = 6, // 网络连接已断开，完全无法沟通
  };

  enum
  {
    NQ_EXCELLENT_INDEX = 0,
    NQ_GOOD_INDEX = 1,
    NQ_POOR_INDEX = 2,
    NQ_BAD_INDEX = 3,
    NQ_VBAD_INDEX = 4
  };

  static const char* parseNetworkQuality(uint32_t eNetworkQuality)
  {
    switch (eNetworkQuality)
    {
      case NQ_UNKNOWN:
        return "NQ_UNKNOWN";
      case NQ_EXCELLENT:
        return "NQ_EXCELLENT";
      case NQ_GOOD:
        return "NQ_GOOD";
      case NQ_POOR:
        return "NQ_POOR";
      case NQ_BAD:
        return "NQ_BAD";
      case NQ_VBAD:
        return "NQ_VBAD";
      case NQ_DOWN:
        return "NQ_DOWN";
      default:
        return "NQ_UNKNOWN2";
    }
  }
};

const int32_t kNetworkQualityLevels = 5;
const uint32_t kNetWorkQualitys[kNetworkQualityLevels] = {NetworkQuality::NQ_EXCELLENT,
                                                          NetworkQuality::NQ_GOOD,
                                                          NetworkQuality::NQ_POOR,
                                                          NetworkQuality::NQ_BAD,
                                                          NetworkQuality::NQ_VBAD};


}
