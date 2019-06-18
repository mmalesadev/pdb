#pragma once

#include "RtAudio.h"
#include "systems/audio/AudioTrack.h"

#include <boost/log/trivial.hpp>
#include <future>
#include <mutex>
#include <condition_variable>

namespace Pdb
{

class AudioStream
{
public:
    AudioStream(std::atomic<float>& masterVolume);
    virtual ~AudioStream() { };

    virtual void play() = 0;
    virtual void stop() = 0;
    virtual void play(const AudioTrack& audioTrack) = 0;
    virtual int playCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status) = 0;

    virtual void seek(int offsetInSeconds) = 0;

    virtual int currentPositionInMilliseconds() = 0;

    void setPlayedAudioTrack(AudioTrack* playedAudioTrack) { playedAudioTrack_ = playedAudioTrack; }

    std::string getPlayedAudioTrackName() const {  if(playedAudioTrack_) return playedAudioTrack_->getTrackName(); else return std::string(""); }

    void waitForEnd();

    void pauseToggle();

    void reserve();
    void makeAvailable();
    bool isPausable() const;
    bool isPaused() const { return state_ == State::PAUSED; }
    bool isAvailable() const { return state_ == State::AVAILABLE; }

    void setVolume(float value) { volume_ = value; }
    float getVolume() const { return volume_; }

protected:
    enum class State { AVAILABLE, RESERVED, PLAYING, PAUSED };

    AudioTrack* playedAudioTrack_;
    
    std::condition_variable finishedPlayingCondVar_;

    std::unique_ptr<RtAudio> rtAudio_;
    RtAudio::StreamParameters parameters_;
    unsigned int sampleRate_;
    unsigned int bufferFrames_;

    State state_;
    std::atomic<float>& masterVolume_;
    float volume_;
    
    std::mutex mutex_;
};

int playCb(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *userData);

}