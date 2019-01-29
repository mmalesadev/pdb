#ifndef _AUDIO_PLAYER_
#define _AUDIO_PLAYER_
#include <ao/ao.h>
#include <mpg123.h>
#include "AudioTrack.h"
#include <vector>

#define BITS 8

class AudioPlayer
{
public:
    AudioPlayer();
    ~AudioPlayer();

    void run(AudioTrack & track);

private:

    void init();
    void open(AudioTrack & track);
    void getDecodingFormat();
    void setOutputFormat();
    void openOutputDevice();
    void play();
    void destroy();

    std::vector<AudioTrack> tracks_;

    mpg123_handle * mh_;
    unsigned char * buffer_;
    size_t buffer_size_;
    size_t done_;
    int err_;

    int driver_;
    ao_device * dev_;

    ao_sample_format format_;
    int channels_, encoding_;
    long rate_;
};


#endif
