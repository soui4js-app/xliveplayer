#pragma once

#include <transvod-i.h>

#include <atomic>
#include <list>
#include <mutex>
#include <SDL.h>
#include <vector>
#include <interface/obj-ref-i.h>
#include <helper/obj-ref-impl.hpp>

using namespace SOUI;

VOD_NS_BEGIN

class SdlAudioRender: public TObjRefImpl<IObjRef> {
 public:
  SdlAudioRender();
  ~SdlAudioRender();

  void stop();
  void reset(const AudioInfo* audioInfo);
  void setVolume(float volPercent);
  BOOL setOutputDevice(const std::string& devName);
  void onRenderFrame(IAVframe* frame);
  bool isMute(void) {return m_isMute;}

 private:
  static void fill_audio(void* udata, Uint8* stream, int len);
  void inner_audio_fill(Uint8* stream, int len);

  void uninit(bool bFinal = false);
  BOOL init(const std::string& devName, const AudioInfo* audioInfo);

  bool m_needReset;
  std::string m_pendingDevName;
  std::string m_deviceName;
  AudioInfo m_audioInfo;
  SDL_AudioDeviceID m_deviceID;

  bool m_playable;
  bool m_isMute;
  std::atomic_int m_volume;
  uint32_t m_audioRate;

  std::mutex m_mutex;
  SAutoRefPtr<IAVframe>            m_renderingFrame;
  std::list<SAutoRefPtr<IAVframe>> m_pendingAudioFrames;
};

VOD_NS_END