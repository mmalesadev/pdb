#pragma once
#include "systems/audio/AudioStream.h"
#include "AudioFile.h"

namespace Pdb
{

class AudioStreamWav : public AudioStream
{
public:
    AudioStreamWav(float& masterVolume);

    void play(const AudioTrack& audioTrack) override;
    int playCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, 
        double streamTime, RtAudioStreamStatus status) override;

    void seek(int offsetInSeconds) override;

private:
    AudioFile<double> audioFile_;
    unsigned int nProcessedSamples_ = 0;
};

}