#pragma once

#include "RtAudio.h"
#include "systems/audio/AudioTrack.h"

#include <boost/log/trivial.hpp>
#include <future>

namespace Pdb
{

class AudioStream
{
public:
    AudioStream(float& masterVolume);
    virtual ~AudioStream() { };

    virtual void play(const AudioTrack& audioTrack) = 0;
    virtual int playCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status) = 0;

    virtual void seek(int offsetInSeconds) = 0;

    void pauseToggle();

    bool isPaused() const { return state_ == State::PAUSED; }
    bool isAvailable() const { return !rtAudio_->isStreamRunning() && !isPaused(); }

    void setVolume(float value) { volume_ = value; }
    float getVolume() const { return volume_; }

    std::future<AudioStream*> getAudioStreamFuture() { return audioStreamPromise_.get_future(); }

protected:
    enum class State { AVAILABLE, PLAYING, PAUSED };

    std::unique_ptr<RtAudio> rtAudio_;
    RtAudio::StreamParameters parameters_;
    unsigned int sampleRate_;
    unsigned int bufferFrames_;

    State state_;
    float& masterVolume_;
    float volume_;

    std::promise<AudioStream*> audioStreamPromise_;
};

int playCb(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *userData);

}