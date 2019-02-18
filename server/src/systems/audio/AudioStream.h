#pragma once

#include "RtAudio.h"
#include "systems/audio/AudioTrack.h"

class AudioStream
{
public:
    AudioStream();
    virtual ~AudioStream() { };

    virtual void play(const AudioTrack& audioTrack) = 0;
    virtual int playCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status) = 0;

    bool isAvailable() const { return !rtAudio_->isStreamOpen(); };
    bool isRunning() const { return rtAudio_->isStreamRunning(); };

protected:
    std::unique_ptr<RtAudio> rtAudio_;
    RtAudio::StreamParameters parameters_;
    unsigned int sampleRate_;
    unsigned int bufferFrames_;
};

int playCb(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *userData);