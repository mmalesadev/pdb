#include "AudioStreamMp3.h"

namespace Pdb
{

AudioStreamMp3::AudioStreamMp3()
{
    mpg123_init();
    int err;
    mh_ = mpg123_new(NULL, &err);
    mp3DecoderOutputBufferSize_ = mpg123_outblock(mh_);
    mp3DecoderOutputBuffer_ = (unsigned char *) malloc(mp3DecoderOutputBufferSize_ * sizeof(unsigned char));
}

AudioStreamMp3::~AudioStreamMp3()
{
    free(mp3DecoderOutputBuffer_);
    mpg123_close(mh_);
    mpg123_delete(mh_);
    mpg123_exit();
}

void AudioStreamMp3::play(const AudioTrack& audioTrack)
{
    std::string path = "../data/" + audioTrack.getFileName();
    mpg123_open(mh_, path.c_str());
    mpg123_getformat(mh_, &rate_, &channels_, &encoding_);
    nPlayedFrames_ = 0;
    
    parameters_.nChannels = channels_;
    parameters_.firstChannel = 0;
    sampleRate_ = rate_;
    bufferFrames_ = 256;

    rtAudio_->openStream(&parameters_, NULL, RTAUDIO_SINT16,
        sampleRate_, &bufferFrames_, &playCb, (void*) this);
    rtAudio_->startStream();
}

int AudioStreamMp3::playCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, 
    double streamTime, RtAudioStreamStatus status)
{
    if ((nPlayedFrames_ * 2) % mp3DecoderOutputBufferSize_ == 0)
    {
        nPlayedFrames_ = 0;
        if (mpg123_read(mh_, mp3DecoderOutputBuffer_, mp3DecoderOutputBufferSize_, &done_) != MPG123_OK)
        {
            return 1;
        }
    }
    
    int16_t* outBuffer = static_cast<int16_t*>(outputBuffer);
    int16_t* mp3DecoderOutputBuffer = reinterpret_cast<int16_t*>(mp3DecoderOutputBuffer_);

    for (int i = 0; i < nBufferFrames; ++i)
    {
        *outBuffer++ = *(mp3DecoderOutputBuffer + nPlayedFrames_);
        mp3DecoderOutputBuffer++;
    }
    nPlayedFrames_ += nBufferFrames; 

    return 0;
}

}