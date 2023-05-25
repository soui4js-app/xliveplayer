//
// Created by Administrator on 2020/6/8.
//

#pragma once

namespace transvod {

enum  NetRequestStatus {

  NETREQ_STATUS_INIT = 0,
  NETREQ_STATUS_HOST_RESOLVE = 1,
  NETREQ_STATUS_CONNECTING = 2,
  NETREQ_STATUS_CONNECTED = 3,
  NETREQ_STATUS_CONNECTION_FAILED = 4,
  NETREQ_STATUS_CONNECTION_CLOSED = 5,
  NETREQ_STATUS_RECV_TIMEOUT = 6,
  NETREQ_STATUS_HTTP_RESPONSE = 7,          /**网络层http请求，服务器返回了response */
  NETREQ_STATUS_PULL_STREAM = 8,            /**网络请求正在拉取数据流数据*/
  NETREQ_STATUS_PULL_PEER_P2P = 9,          /**如果支持p2功能，网络请求正处理p2p拉流状态, 否则无效*/
  NETREQ_STATUS_RESPONSE_TIMEOUT = 10,      /**等待http回复超时*/
};

struct NetRequestStatusInfo {
  std::string m_url;
  std::string m_serverIp;
  bool m_connected;
  int32_t m_httpCode;

  NetRequestStatusInfo() : m_serverIp(""),
  m_connected(false), m_httpCode(-1), m_url("") {

  }
};

}
