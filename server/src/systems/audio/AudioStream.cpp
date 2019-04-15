#include "AudioStream.h"
#include <boost/log/trivial.hpp>

namespace Pdb
{

AudioStream::AudioStream(float& masterVolume) : masterVolume_(masterVolume), state_(State::AVAILABLE)
{
    rtAudio_ = std::make_unique<RtAudio>();
    int nDevices = rtAudio_->getDeviceCount();
    if (nDevices < 1) {
        BOOST_LOG_TRIVIAL(error) << "No audio devices found!";
        exit(0);
    }
    parameters_.deviceId = rtAudio_->getDefaultOutputDevice();
}

void AudioStream::pauseToggle()
{
    if(state_ == State::PAUSED)
    {
        BOOST_LOG_TRIVIAL(info) << "Resuming stream.";
        rtAudio_->startStream();
        state_ = State::PLAYING;
    }
    else
    {
        BOOST_LOG_TRIVIAL(info) << "Pausing stream.";
        state_ = State::PAUSED;
        rtAudio_->abortStream();
    }
}

int playCb(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *userData)
{
    return ((AudioStream*)userData)->playCallback(outputBuffer, inputBuffer, nBufferFrames, streamTime, status);
}

}