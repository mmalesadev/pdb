#ifndef _AUDIO_PLAYER_
#define _AUDIO_PLAYER_
#include <mpg123.h>
#include "AudioTrack.h"
#include "RtAudio.h"
#include <vector>
#include <memory>
#include "soundfile.h"

#define BITS 8

class AudioPlayer
{
public:
    AudioPlayer();
    ~AudioPlayer();

    void initMP3(AudioTrack & audioTrack);
    void initWAV(AudioTrack & audioTrack);

    void playMP3(AudioTrack & audioTrack);
    void playWAV(AudioTrack & audioTrack);
    int mp3Callback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status);
    int wavCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status);

private:
    void destroy();

    std::vector<AudioTrack> tracks_;

    mpg123_handle * mh_;
    unsigned char * mp3DecoderOutputBuffer_;
    size_t mp3DecoderOutputBufferSize;
    size_t done_;
    int err_;

    int channels_, encoding_;
    long rate_;

    std::unique_ptr<RtAudio> rtAudio_;
    RtAudio::StreamParameters parameters_;
    unsigned int sampleRate_;
    unsigned int bufferFrames_;
    
    unsigned int nPlayedFrames_;
};


#endif
