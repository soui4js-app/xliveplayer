#include "stdafx.h"
#include "SdlAudioRender.h"

VOD_NS_BEGIN

SdlAudioRender::SdlAudioRender()
: m_playable(false) 
, m_isMute(false)
, m_audioRate(0) {
  memset(&m_audioInfo, 0, sizeof(m_audioInfo));
  setVolume(1.0f);
}

SdlAudioRender::~SdlAudioRender() {
  uninit();
}

void SdlAudioRender::stop() {
  uninit(true);
}

void SdlAudioRender::reset(const AudioInfo* audioInfo) {
  if (!m_needReset) {
    m_needReset = true;
    m_pendingDevName = m_deviceName;
  }
  m_audioInfo = *audioInfo;

  uninit();
}

void SdlAudioRender::fill_audio(void* udata, Uint8* stream, int len) {
  SdlAudioRender* thiz = (SdlAudioRender*)udata;
  thiz->inner_audio_fill(stream, len);
}

void SdlAudioRender::onRenderFrame(IAVframe* frame) {
    AudioInfo info = { 0 };

    info.channels = frame->audioChannels();
    info.sampleRate = frame->audioSampleRate();
    info.channel_layout = frame->audioChannelLayout();
    info.samples = frame->audioSamples();
    if (memcmp(&m_audioInfo, &info, sizeof(info)) != 0) {
        m_needReset = true;
        m_audioInfo = info;
        m_pendingDevName = m_deviceName;
    }

  if (m_needReset) {
    m_needReset = false;
    auto audioInfo = m_audioInfo;
    init(m_pendingDevName, &audioInfo);
  }

  // 如果打开播放设备失败，则不继续播放音频
  if (!m_playable) {
    return;
  }


  bool doClear{false};
  uint32_t bufSize = 0;
  {
    std::lock_guard<std::mutex> lk(m_mutex);
    bufSize = m_pendingAudioFrames.size();
    // 当堆积的音频量达到1s时，清空当前缓存
    if (bufSize > m_audioRate) {
      doClear = true;
      auto back_frame = m_pendingAudioFrames.back();
      m_pendingAudioFrames.clear();
      m_pendingAudioFrames.emplace_back(back_frame);
    }

    m_pendingAudioFrames.push_back(frame);
  }
}

void SdlAudioRender::setVolume(float volPercent) {
  double realPercent = max(min(1.0, volPercent), 0.0);
  int volume = (int)(realPercent * 128);

  m_isMute = volume == 0;
  m_volume.store(volume, std::memory_order_release);
}

BOOL SdlAudioRender::setOutputDevice(const std::string& devName) {

  // 如果该设备已经被打开了，则不再重复打开
  if (m_deviceID && devName == m_deviceName) {
    return TRUE;
  }

  m_needReset = true;
  m_pendingDevName = devName;

  uninit();
  return TRUE;
}

void SdlAudioRender::inner_audio_fill(Uint8* stream, int len) {
    SDL_memset(stream, 0, len);

    SAutoRefPtr<IAVframe> frame;
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        if (!m_pendingAudioFrames.empty()) {
            frame = m_pendingAudioFrames.front();
            m_renderingFrame = frame;
            m_pendingAudioFrames.pop_front();
        }
    }

    if (frame == nullptr) {
        return;
    }

    auto volume = m_volume.load(std::memory_order_acquire);
    if (volume == 0) {
        return;
    }

    int copySize = min(len, (int)frame->length());
    if (volume == 128) {
        memcpy(stream, frame->getData(), copySize);
    }
    else {
        SDL_MixAudioFormat(stream, frame->getData(),
            AUDIO_S16SYS, copySize, volume);
    }
}

void SdlAudioRender::uninit(bool bFinal) {
  if (bFinal && m_needReset) {
    m_needReset = false;
  }

  if (m_deviceID) {
    m_playable = false;

    SDL_CloseAudioDevice(m_deviceID);

    m_deviceID = 0;
    m_deviceName.clear();
  }

  std::lock_guard<std::mutex> lk(m_mutex);
  m_pendingAudioFrames.clear();
  m_renderingFrame = nullptr;
}

BOOL SdlAudioRender::init(const std::string& devName,
                                  const AudioInfo* audioInfo) {
  SDL_AudioSpec wanted = {};

  wanted.freq = audioInfo->sampleRate;
  wanted.format = AUDIO_S16SYS;
  wanted.channels = audioInfo->channels;
  wanted.silence = 0;
  wanted.samples = audioInfo->samples;
  wanted.callback = &SdlAudioRender::fill_audio;
  wanted.userdata = this;
  m_audioRate = (uint32_t)ceil(((double)audioInfo->sampleRate)/audioInfo->samples);

  auto res =
    SDL_OpenAudioDevice(devName.empty() ? nullptr : devName.c_str(), false, &wanted, NULL, 0);
  if (res < 2) {
    return FALSE;
  }
  m_deviceName = devName;
  m_deviceID = res;

  m_playable = true;


  SDL_PauseAudioDevice(m_deviceID, 0);
  return TRUE;
}

VOD_NS_END
