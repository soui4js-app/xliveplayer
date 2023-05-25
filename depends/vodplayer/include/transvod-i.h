#pragma once

#include <transvod-def.h>
#include <stdint.h>
#include <Unknwn.h>
#include <core/types/DataSourceFormat.h>
#include <core/types/CachePolicy.h>
#include <core/types/UrlProtocol.h>
#include <core/types/ErrorCode.h>
#include <core/types/StaticsType.h>
#include <core/types/PlayerState.h>
#include <core/types/SeiDseVer.h>
VOD_NS_BEGIN

enum AVPixelFormat {
    //和libavutil/pixfmt.h定义保持一致。
    AV_PIX_FMT_NONE = -1,
    AV_PIX_FMT_YUV420P,   ///< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)
    AV_PIX_FMT_YUYV422,   ///< packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr
    AV_PIX_FMT_RGB24,     ///< packed RGB 8:8:8, 24bpp, RGBRGB...
    AV_PIX_FMT_BGR24,     ///< packed RGB 8:8:8, 24bpp, BGRBGR...
    AV_PIX_FMT_GRAY8 = 8, ///<        Y        ,  8bpp
    AV_PIX_FMT_NV12 = 23, ///< planar YUV 4:2:0, 12bpp, 1 plane for Y and 1 plane for the UV components, which are
                          ///< interleaved (first byte U and the following byte V)
    AV_PIX_FMT_NV21,      ///< as above, but U and V bytes are swapped

    AV_PIX_FMT_ARGB, ///< packed ARGB 8:8:8:8, 32bpp, ARGBARGB...
    AV_PIX_FMT_RGBA, ///< packed RGBA 8:8:8:8, 32bpp, RGBARGBA...
    AV_PIX_FMT_ABGR, ///< packed ABGR 8:8:8:8, 32bpp, ABGRABGR...
    AV_PIX_FMT_BGRA, ///< packed BGRA 8:8:8:8, 32bpp, BGRABGRA...
};


typedef struct _AudioSendStamp {
    uint32_t mSendStampMs;
    uint32_t mCaptureStampMs;
} AudioSendStamp;

typedef struct _AudioInfo {
    int channels;
    int sampleRate;
    uint64_t channel_layout;
    int samples;
} AudioInfo;

typedef struct _VideoInfo {
    int width;
    int height;
} VideoInfo;

typedef struct _VideoDecoderParam {
    AVPixelFormat format;
    int width;
    int height;
} VideoDecoderParam;

typedef struct _AudioDecoderParam {
    int channels;
    int channel_layout;
    int sample_rate;
    int profile;
} AudioDecoderParam;

enum VideoRenderMode {
    VIDEO_RENDER_MODE_FILL = 0,             // 拉伸全屏（画面会变形）
    VIDEO_RENDER_MODE_ASPECT_FIT = 1,       // 自适应全屏（视频维持原尺寸放大到全屏，其他部分使用黑边填充）
    VIDEO_RENDER_MODE_CLIP_TO_BOUNDS = 2,   // 裁减全屏（画面放大到屏幕尺寸，超出屏幕范围的裁剪掉）
    VIDEO_RENDER_MODE_ORIGINAL = 3,         // 按视频原尺寸渲染视频（多余部分使用黑边填充）
};

struct VideoRenderConfig {
    HWND hWnd;
    VideoRenderMode mode;
};

struct RenderRectParam {
    bool isRelative;
    
    /**
     * 用于描述渲染区域Rect的参数，则视频图像只会在该区域里进行渲染。
     * 有两种模式：
     * 1. 绝对模式（isRelative值为false），则下面的参数填的是实际的
     *    绝对坐标与具体的区域长宽, 比如 {x:0, y:20, w:1280, h:720},
     *    适用于窗口大小固定的情况。
     * 2. 相对坐标模式（isRelative值为true），则下面的参数填的是渲染
     *    区域相对实际窗口的比值，则下述参数的取值应该在（0.0 ~ 1.0）
     *    之间。比如 {x:0.0, y:0.1, w:1.0, h:1.0}。
     * 
     * 如果播放过程中，播放窗口大小是变化的，建议选择相对坐标模式。
     */
    float x;      // 渲染的图像的左上角x轴相对于窗口左上角的相对位置比率
    float y;      // 渲染的图像的左上角y轴相对于窗口左上角的相对位置的比率
    float width;  // 渲染的图像的宽度相对窗口宽度的比率
    float height; // 渲染的图像的高度相对窗口高度的比率
};

typedef enum {
    DEV_GUID,       // 设备的guid
    DEV_UTF8_NAME,  // 设备的utf8名称
}EDeviceParam;

struct AudioDeviceParam {
    /**
     * eType == DEV_GUID，则deviceID类型是 LPGUID
     * eType == DEV_UTF8_NAME，则deivceID是包含UTF8格式设备名称的char*
     */
    EDeviceParam eType;
    void* deviceID;
};

#undef INTERFACE
#define INTERFACE IAVframe
DECLARE_INTERFACE_(IAVframe, IUnknown) {
    STDMETHOD_(uint8_t, ssrc)(THIS) CONST PURE;
    STDMETHOD_(int, type)(THIS) CONST PURE;
    STDMETHOD_(int, netCodec)(THIS) CONST PURE;
    STDMETHOD_(int, playTaskID)(THIS) CONST PURE;
    STDMETHOD_(int, getDecoderType)(THIS) CONST PURE;

    STDMETHOD_(BOOL, isKeyFrame)(THIS) CONST PURE;
    STDMETHOD_(uint32_t, idx)(THIS) CONST PURE;
    STDMETHOD_(uint32_t, capStamp)(THIS) CONST PURE;
    STDMETHOD_(uint32_t, recvStamp)(THIS) CONST PURE;
    STDMETHOD_(uint32_t, decodedStamp)(THIS) CONST PURE;
    STDMETHOD_(uint32_t, width)(THIS) CONST PURE;
    STDMETHOD_(uint32_t, height)(THIS) CONST PURE;
    STDMETHOD_(uint32_t, audioChannels)(THIS) CONST PURE;
    STDMETHOD_(uint32_t, audioChannelLayout)(THIS) CONST PURE;
    STDMETHOD_(uint32_t, audioSampleRate)(THIS) CONST PURE;
    STDMETHOD_(uint32_t, audioSamples)(THIS) CONST PURE;
    STDMETHOD_(uint32_t, pts)(THIS) CONST PURE;
    STDMETHOD_(uint32_t, dts)(THIS) CONST PURE;
    STDMETHOD_(uint64_t, streamId)(THIS) CONST PURE;
    STDMETHOD_(uint8_t *, getSeiOrDse)(THIS_ int *pLen) CONST PURE;
    STDMETHOD_(transvod::SeiDseVer, getSeiDseVer)(THIS) CONST PURE;
    STDMETHOD_(uint8_t *, getData)(THIS) CONST PURE;
    STDMETHOD_(uint32_t, length)(THIS) CONST PURE;
    STDMETHOD_(AVPixelFormat, format)(THIS) CONST PURE;
    STDMETHOD_(const int *, lineSize)(THIS) CONST PURE;

    STDMETHOD_(uint32_t, getPendingDecodeStamp)(THIS) CONST PURE;
    STDMETHOD_(void, setPendingDecodeStamp)(THIS_ uint32_t ts) PURE;
    STDMETHOD_(uint32_t, getBeforeDecodeStamp)(THIS) CONST PURE;
    STDMETHOD_(void, setBeforeDecodeStamp)(THIS_ uint32_t ts) PURE;
    STDMETHOD_(uint32_t, getAfterDecodeStamp)(THIS) CONST PURE;
    STDMETHOD_(void, setAfterDecodeStamp)(THIS_ uint32_t ts) PURE;

    STDMETHOD_(uint32_t, getPendingRenderStamp)(THIS) CONST PURE;
    STDMETHOD_(void, setPendingRenderStamp)(THIS_ uint32_t ts) PURE;
    STDMETHOD_(uint32_t, getBeforeRenderStamp)(THIS) CONST PURE;
    STDMETHOD_(void, setBeforeRenderStamp)(THIS_ uint32_t ts) PURE;
    STDMETHOD_(uint32_t, geAfterRenderStamp)(THIS) CONST PURE;
    STDMETHOD_(void, setAfterRenderStamp)(THIS_ uint32_t ts) PURE;
    STDMETHOD_(void, setDecoderCreateDelay)(THIS_ uint32_t ts) PURE;
    STDMETHOD_(uint32_t, getDecoderCreateDelay)(THIS) CONST PURE;

    STDMETHOD_(void, setSerial)(THIS_ uint32_t serial) PURE;
    STDMETHOD_(uint32_t, getSerial)(THIS) CONST PURE;
    STDMETHOD_(uint32_t, getJitterBufferSerial)(THIS) CONST PURE;

    STDMETHOD_(BOOL, isFirstFrame)(THIS) CONST PURE;
    STDMETHOD_(BOOL, isVideo)(THIS) CONST PURE;
    STDMETHOD_(BOOL, isHardDecoder)(THIS) CONST PURE;
};

#undef INTERFACE
#define INTERFACE IAVStream
DECLARE_INTERFACE_(IAVStream, IUnknown) {
    STDMETHOD_(uint32_t, startTimeMs)(THIS) CONST PURE;
    STDMETHOD_(uint32_t, totalDurationMs)(THIS) CONST PURE;
    STDMETHOD_(uint32_t, streamIndex)(THIS) CONST PURE;
    STDMETHOD_(uint32_t, YYNetCodec)(THIS) CONST PURE;
    STDMETHOD_(uint32_t, totalFrame)(THIS) CONST PURE; // number of frames in this stream if known or 0
    STDMETHOD_(uint32_t, width)(THIS) CONST PURE;
    STDMETHOD_(uint32_t, height)(THIS) CONST PURE;
    STDMETHOD_(int, rotate)(THIS) CONST PURE;
    STDMETHOD_(int, playTaskID)(THIS) CONST PURE;
    STDMETHOD_(BOOL, isVideoStream)(THIS) CONST PURE;
    STDMETHOD_(BOOL, isAudioStream)(THIS) CONST PURE;
    STDMETHOD_(LPCSTR, title)(THIS) CONST PURE;
};

#undef INTERFACE
#define INTERFACE ITransVodListener
DECLARE_INTERFACE(ITransVodListener) {
    STDMETHOD_(void, onAVStream)(THIS_ LPCSTR url, const IAVStream **streams, int nStreamCount, const char* metaData, BOOL isLastFrame) PURE;

    STDMETHOD_(void, onAudioFormatChanged)(THIS_ LPCSTR url, const AudioInfo *mediaInfo) PURE;

    STDMETHOD_(void, onVideoFormatChanged)(THIS_ LPCSTR url, const VideoInfo *mediaInfo) PURE;    

    STDMETHOD_(BOOL, onGetVideoDecoderParam)(THIS_ VideoDecoderParam *pParam) PURE;

    STDMETHOD_(BOOL, onGetAudioDecoderParam)(THIS_ AudioDecoderParam *pParam) PURE;

    STDMETHOD_(void, onVideoFrame)(THIS_ LPCSTR url, IAVframe *frame) PURE;
    STDMETHOD_(void, onAudioFrame)(THIS_ LPCSTR url, IAVframe *frame) PURE;

    STDMETHOD_(void, onStateChanged)(THIS_ LPCSTR url, transvod::PlayerState state, transvod::ErrorReason reason) PURE;

    STDMETHOD_(void, onTotalTime)(THIS_ LPCSTR url, uint32_t totalTime) PURE;

    STDMETHOD_(void, onLoadingChanged)(THIS_ LPCSTR url, uint32_t percent) PURE;

    STDMETHOD_(void, onPlayedTimeChanged)(THIS_ LPCSTR url, uint32_t playedTime) PURE;

    STDMETHOD_(void, onCacheTimeChanged)(THIS_ LPCSTR url, uint32_t cacheTime) PURE;

    STDMETHOD_(void, onResourceTotalSize)(THIS_ LPCSTR url, uint32_t totalSize) PURE;

    STDMETHOD_(void, onStatics)(THIS_ LPCSTR url, transvod::PlayerStaticsType type, LPCSTR data, uint32_t len) PURE;

    STDMETHOD_(void, onError)(THIS_ LPCSTR url, transvod::ErrorCode errCode, int statusCode) PURE;

    STDMETHOD_(void, onDownloadSpeed)(THIS_ LPCSTR url, uint32_t costTime, uint32_t speed, BOOL isComplete) PURE;

    STDMETHOD_(void, onCatonTimes)(THIS_ LPCSTR url, uint32_t * catonTimes, int nSize) PURE;

    STDMETHOD_(void, onResolution)(THIS_ LPCSTR url, uint32_t wid, uint32_t hei) PURE;

    STDMETHOD_(void, onPlayerSeekStart)(THIS_ LPCSTR url, uint32_t seekTime, uint64_t seekId) PURE;
    
    STDMETHOD_(void, onPlayedSeekFinish)
    (THIS_ LPCSTR url, uint32_t seekTime) PURE;

    STDMETHOD_(void, onPlayedEndOneLoop)(THIS_ LPCSTR url) PURE;

    STDMETHOD_(void, onVideoCacheCompleted)(THIS_ LPCSTR url, LPCSTR pszCachePath) PURE;

    STDMETHOD_(void, onPlayerNetRequestStatus)(THIS_ LPCSTR url, int32_t status, int32_t networkQuality, LPCSTR svrIp, BOOL bConnnected, int32_t nhttpCode) PURE;

    STDMETHOD_(void, onMediaInfo)(THIS_ LPCSTR url, BOOL hasAudio, BOOL hasVideo) PURE;

    STDMETHOD_(void, onQualityReport)
    (THIS_ uint32_t alt, uint32_t rtf, uint32_t rvf, uint32_t rvff, uint32_t rcf, uint32_t rcff) PURE;
    STDMETHOD_(void, onBitrateReport)
        (THIS_ uint32_t audioBitrate,uint32_t videoBitrate) PURE;

    STDMETHOD_(void, onRecordStart)(THIS_ LPCSTR filename,int errCode) PURE;
    STDMETHOD_(void, onRecordStop)(THIS_ LPCSTR filename, int errCode) PURE;
};

typedef void (*funWriteLogCallback)(const char *func, int line, int level, const char *tag, const char *text);
typedef void (*funStatisticCallback)(const int taskId, const int type, const char *text, const int length);


typedef enum {
    pending,
    before_render,
    after_render,
}RENDERSTATE;

#undef INTERFACE
#define INTERFACE IUrlProperty
DECLARE_INTERFACE_(IUrlProperty, IUnknown) {
    STDMETHOD_(BOOL, putValue)(THIS_ LPCSTR pszKey, LPCSTR pszValue) PURE;
    STDMETHOD_(LPCSTR, getValue)(THIS_ LPCSTR pszKey) CONST PURE;
    STDMETHOD_(size_t, getCount)(THIS) CONST PURE;
    STDMETHOD_(size_t, getKeys)(THIS_ LPCSTR * pKeys, size_t count) CONST PURE;
};

#undef INTERFACE
#define INTERFACE ITransVod
DECLARE_INTERFACE_(ITransVod, IUnknown) {
    STDMETHOD_(BOOL, setHardwareDocoderConfig)(THIS_ LPCSTR pszJson) PURE;

    STDMETHOD_(int, scale)(THIS_ const IAVframe *src, int nWid, int nHei, AVPixelFormat dstFmt, IAVframe **ppOutput) PURE;
    STDMETHOD_(HRESULT, CreateUrlProperty)(THIS_ IUrlProperty **ppRet) PURE;
    STDMETHOD_(void, play)
    (THIS_ LPCSTR url,
     transvod::UrlProtocol urlProtocol,
     transvod::DataSourceFormat sourceFormat,
     transvod::CachePolicy cachePolicy,
     int playTaskID,
     BOOL bLive,
     uint64_t startStamp,
     const IUrlProperty *pUrlProperty) PURE;

    STDMETHOD_(void, pause)(THIS) PURE;

    STDMETHOD_(void, resume)(THIS) PURE;

    STDMETHOD_(void, stop)(THIS_ BOOL bRelease) PURE;

    STDMETHOD_(void, seekTo)(THIS_ uint32_t position) PURE;

    STDMETHOD_(void, error)(THIS_ transvod::ErrorCode errCode) PURE;

    STDMETHOD_(void, setCacheTime)(THIS_ long minCacheMs, long maxCacheMs, long minCatonMs) PURE;

    STDMETHOD_(void, setNumberOfLoops)(THIS_ int numberOfLoops) PURE;

    STDMETHOD_(void, networkChanged)(THIS_ int netstate) PURE;

    STDMETHOD_(void, reportAudioSendStamp)(THIS_ const AudioSendStamp *audioSendStamps, int nSize) PURE;

    STDMETHOD_(void, setQualityReportInterval)(THIS_ int nIntervalMs) PURE;

    STDMETHOD_(void, feedbackRenderState)(THIS_ IAVframe * avFrame, RENDERSTATE state) PURE;

    STDMETHOD_(transvod::PlayerState, getPlayerState)(THIS) PURE;

    STDMETHOD_(void, setVideoFilter)(THIS_ const char* filterDesc) PURE;

    STDMETHOD_(void, selectAudioTrack)(THIS_ int iTrack) PURE;

    STDMETHOD_(BOOL, startMp4Muxer)(THIS_ const char* pszFileName) PURE;
    STDMETHOD_(void, stopMp4Muxer)(THIS) PURE;
};

#ifdef __cplusplus
extern "C" {
#endif

typedef ITransVod *ITransVodPtr;

ITransVodPtr TRANSVOD_API CreateTransVod(const char *model,
                                         const char *sysver,
                                         uint32_t playerContextId,
                                         const char *cacheDir,
                                         ITransVodListener *pListener);
void TRANSVOD_API TransVod_GetVersion(char *pBuf, int nBufLen);

//删除播放器中的单例，释放分配的内存，该方法限测试使用。释放后不能继续播放。
void TRANSVOD_API TransVod_FreeSingletons();

void TRANSVOD_API TransVod_SetWriteLogCallback(funWriteLogCallback cbLogWriter);
void TRANSVOD_API TransVod_SetStatisticsCallback(funStatisticCallback cbStatistics);

BOOL TRANSVOD_API TransVod_IsSupportMedia(const char *pszUrl, int *errCode);

#ifdef __cplusplus
}
#endif

VOD_NS_END
