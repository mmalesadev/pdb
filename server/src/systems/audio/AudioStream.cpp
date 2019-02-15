#include "AudioStream.h"
#include "spdlog/spdlog.h"

AudioStream::AudioStream()
{
    rtAudio_ = std::make_unique<RtAudio>();
    int nDevices = rtAudio_->getDeviceCount();
    if (nDevices < 1) {
        spdlog::get("console")->error("No audio devices found!");
        exit(0);
    }
    parameters_.deviceId = rtAudio_->getDefaultOutputDevice();
}

int playCb(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *userData)
{
    return ((AudioStream*)userData)->playCallback(outputBuffer, inputBuffer, nBufferFrames, streamTime, status);
}