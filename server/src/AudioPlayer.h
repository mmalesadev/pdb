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

    void playMP3(AudioTrack & track);
    void playWAV(AudioTrack & track);
    int mp3Callback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status);

private:
    void init();
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
