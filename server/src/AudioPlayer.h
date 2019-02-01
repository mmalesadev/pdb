#ifndef _AUDIO_PLAYER_
#define _AUDIO_PLAYER_
#include <mpg123.h>
#include "AudioTrack.h"
#include "RtAudio.h"
#include <vector>
#include <memory>

#define BITS 8

class AudioPlayer
{
public:
    AudioPlayer();
    ~AudioPlayer();

    void run(AudioTrack & audioTrack);

private:

    void init();
    void open(AudioTrack & track);
    void getDecodingFormat();
    void play();
    void destroy();

    std::vector<AudioTrack> tracks_;

    mpg123_handle * mh_;
    unsigned char * buffer_;
    size_t buffer_size_;
    size_t done_;
    int err_;

    int channels_, encoding_;
    long rate_;

    std::unique_ptr<RtAudio> rtAudio_;
    RtAudio::StreamParameters parameters_;
    unsigned int sampleRate_;
    unsigned int bufferFrames_;
};


#endif
