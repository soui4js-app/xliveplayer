//
// Created by YY on 2020/8/3.
//

#ifndef TRANSVOD_YYNETCODEC_H
#define TRANSVOD_YYNETCODEC_H

namespace transvod {
    // YY NET CODEC
    enum YYNetCodec{
        NET_UNKNOWN_CODEC = -1000,
        NET_AMRWB = 3,
        NET_SILK_16K = 2,
        NET_SILK_24K = 22,
        NET_AAC_128K = 35,
        NET_AAC_160K = 36,
        NET_AAC_192K = 37,
        NET_AAC_ELD = 38,
        NET_AAC_ELD_SBR = 39,
        NET_AAC_48K_STEREO = 40,
        NET_AAC_44K_MONO = 41,
        NET_AAC_48K_MONO = 42,
        NET_AAC_ELD_16Khz_MONO = 43, // for connect mic
        NET_AAC_ELD_32Khz_MONO = 44, // for connect mic
        NET_AAC_64K_MONO = 45,
        NET_AAC_HEV1_48K = 46,
        NET_OPUS_24K4 = 50,
        NET_OPUS_48KHZ = 51,
        NET_OPUS_COMMON = 52,
        NET_AACPLUS = 53,
        NET_SPEEX_MODE_8 = 0,
        NET_SPEEX_MODE_4 = 21,
        NET_SPEEX_MODE_2 = 23,
        NET_VIDEO_H264 = 2000,
        NET_VIDEO_VP8 = 2001,
        NET_VIDEO_H265 = 2002,
        NET_VIDEO_MPEG4 = 2003,
        NET_PHONE_1ON1_PROTO = 98,
    };

	enum YYDeCoderType
	{
		//0 - unknow 1 - mediaCodec 2 - ittiam 3 - ffmpeg 4 - videotoolbox
		//5 - qsv 6 - cuvid  7 - dxva 8 - d3d11va
		DeCoder_unknow = 0,
		DeCoder_mediaCodec,
		DeCoder_ittiam,			//软解
		DeCoder_ffmpeg,			//软解
		DeCoder_videotoolbox,
		DeCoder_qsv,			//硬解
		DeCoder_cuvid,			//硬解
		DeCoder_dxva,			//硬解
		DeCoder_d3d11va			//硬解
	};
}
#endif //TRANSVOD_YYNETCODEC_H
