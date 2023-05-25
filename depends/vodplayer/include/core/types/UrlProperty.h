//
// Created by Administrator on 2020/10/20.
//
#pragma once

#include <string>
#include <map>

namespace transvod {

static const char* kUrlPropertySharedUrl = "urlPropSharedUrl";
/** url */
static const char* kUrlPropertyUrl = "urlPropUrl";

/** for p2p use, room id correspond the url played*/
static const char* kUrlPropertyRoomId = "urlPropRoomId";

/** for p2p use, uid of the user*/
static const char* kUrlPropertyUid = "urlPropUid";

/**url流信息中如果包含了视频的宽度，则设置*/
static const char* kUrlPropertyPrefetchWidth = "urlPropWidth";

/**url流信息中如果包含了视频的高度，则设置*/
static const char* kUrlPropertyPrefetchHeight = "urlPropHeight";

/**url流信息中如果包含了视频的编码格式，则设置*/
static const char* kUrlPropertyPrefetchVideoCodec = "urlPropVideoCodec";

// pcdn 模式下切换回cdn模式的卡顿时长阈值
static const char* kPcdnCatonTimeThreshold = "pcdnCatonTimeThreshold";

// pcdn 模式下切换回cdn模式的卡顿次数阈值
static const char* kPcdnCatonCountThreshold = "pcdnCatonCountThreshold";

typedef std::map<std::string, std::string> UrlProperty;
}
