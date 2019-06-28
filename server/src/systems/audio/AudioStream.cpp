#include "AudioStream.h"
#include <boost/log/trivial.hpp>

namespace Pdb
{

AudioStream::AudioStream(std::atomic<float>& masterVolume) : masterVolume_(masterVolume), state_(State::AVAILABLE),
    playedAudioTrack_(nullptr)
{
    rtAudio_ = std::make_unique<RtAudio>();
    int nDevices = rtAudio_->getDeviceCount();
    if (nDevices < 1) {
        BOOST_LOG_TRIVIAL(error) << "No audio devices found!";
        exit(0);
    }
    parameters_.deviceId = rtAudio_->getDefaultOutputDevice();
}

void AudioStream::waitForEnd()
{
    std::unique_lock<std::mutex> lock(mutex_);
    finishedPlayingCondVar_.wait(lock, [&] { return isAvailable(); });
    BOOST_LOG_TRIVIAL(info) << "Finished waiting for AudioStream end.";
}

bool AudioStream::isPausable() const
{
    if (playedAudioTrack_) return playedAudioTrack_->isStandard();
    return false;
}

void AudioStream::pauseToggle()
{
    std::unique_lock<std::mutex> lock(mutex_);

    if(state_ == State::PAUSED)
    {
        BOOST_LOG_TRIVIAL(info) << "Resuming stream: " << playedAudioTrack_->getTrackName();
        rtAudio_->startStream();
        state_ = State::PLAYING;
    }
    else
    {
        BOOST_LOG_TRIVIAL(info) << "Pausing stream: " << playedAudioTrack_->getTrackName();
        state_ = State::PAUSED;
        rtAudio_->abortStream();
    }
}

void AudioStream::reserve()
{
    if (state_ == State::AVAILABLE) state_ = State::RESERVED;
}

void AudioStream::makeAvailable()
{
    state_ = State::AVAILABLE;
}

int playCb(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *userData)
{
    return ((AudioStream*)userData)->playCallback(outputBuffer, inputBuffer, nBufferFrames, streamTime, status);
}

}