#pragma once

#include <mpg123.h>
#include "systems/audio/AudioStream.h"

namespace Pdb
{

class AudioStreamMp3 : public AudioStream 
{
public:
    AudioStreamMp3();
    ~AudioStreamMp3();

    void play(const AudioTrack& audioTrack) override;
    void close() override;
    int playCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, 
        double streamTime, RtAudioStreamStatus status) override;

private:
    mpg123_handle * mh_;
    unsigned char * mp3DecoderOutputBuffer_;
    size_t mp3DecoderOutputBufferSize_;
    size_t nDecodedBytesToProcessLeft_;

    int channels_, encoding_;
    long rate_;

    unsigned int nPlayedFrames_;
    bool doneDecodingMp3_;
};

}