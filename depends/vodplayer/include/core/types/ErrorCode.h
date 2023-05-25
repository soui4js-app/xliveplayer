#pragma once

namespace transvod {

enum  ErrorCode {
    // 网络相关
    ERR_DNSRESOLVE = 1000,        //dns resolve error.
    ERR_HTTP_ERROR = 1001,      //http response error
    ERR_CONNECT = 1002,         //connect error.
    ERR_CONNECTED_TIMEOUT = 1003,  //connected but don't receive anything.
    ERR_RESPONSED_TIMEOUT = 1004, //connected bug receive response timeout.
    ERR_RECV_DATA_TIMEOUT = 1005, //connected but has no data for 30sec for connection.
    ERR_STOP_IN_50MS = 1006,      //player stopped in 50ms
    ERR_P2P_DNSRESOLVE = 1007,    //p2p模块域名解析失败(v6 v4 都失败)
    ERR_P2P_DNSRESOLVE_V4 = 1008, //p2p模块域名解析失败v4失败,v6成功
    ERROR_INVALID_DATA_COMPLETE = 1009,//直播模式下，不该存在数据读取完成事件
    
    /**demux相关的*/
    ERR_PROBE_ERROR = 2000,
    ERR_READ_HEADER_ERROR = 2001,
    ERR_READ_FRAME_ERROR = 2002,
    
    ERR_SEEK_ERROR = 3000,

    ERR_READ_CACHE = 4000,
    ERR_WRITE_CACHE = 4001,

    ERR_CREATE_DEC_FAILED = 5000, // 创建解码器失败
    ERR_DECODE_FAILED = 5001,     // 解码错误
    
    ERR_BAD_RESOURCE = 6000,       // 资源损坏
    RESOURCE_UNAVAILABLE = 6001,
    
    SUCCESS = 10000,
};
}
