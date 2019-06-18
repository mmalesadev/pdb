#pragma once
#include "systems/audio/AudioStream.h"
#include "AudioFile.h"

namespace Pdb
{

class AudioStreamWav : public AudioStream
{
public:
    AudioStreamWav(std::atomic<float>& masterVolume);

    void play() override;
    void stop() override;
    void play(const AudioTrack& audioTrack) override;
    int playCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, 
        double streamTime, RtAudioStreamStatus status) override;

    int currentPositionInMilliseconds() override;

    void seek(int offsetInMilliseconds) override;

private:
    AudioFile<double> audioFile_;
    unsigned int nProcessedSamples_ = 0;
};

}