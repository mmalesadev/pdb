#pragma once
#include "systems/audio/AudioStream.h"
#include "AudioFile.h"

namespace Pdb
{

class AudioStreamWav : public AudioStream
{
public:
    AudioStreamWav();

    void play(const AudioTrack& audioTrack) override;
    void close() override;
    int playCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, 
        double streamTime, RtAudioStreamStatus status) override;

private:
    AudioFile<double> audioFile_;
    unsigned int nProcessedSamples_ = 0;
};

}