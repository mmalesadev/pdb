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
    doneDecodingMp3_= false;
}

void AudioStreamMp3::close()
{
    //rtAudio_->closeStream();
    mpg123_close(mh_);
    doneDecodingMp3_ = false;
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
    std::string path = audioTrack.getFilePath();
    mpg123_open(mh_, path.c_str());
    mpg123_getformat(mh_, &rate_, &channels_, &encoding_);
    nPlayedFrames_ = 0;
    
    parameters_.nChannels = channels_;
    parameters_.firstChannel = 0;
    sampleRate_ = rate_;
    bufferFrames_ = 256;
    doneDecodingMp3_ = false;
    audioStreamPromise_ = std::promise<AudioStream*>();

    BOOST_LOG_TRIVIAL(info) << "Playing mp3 audio stream. Rate: " << rate_ << ", channels: " << channels_ << ", encoding: " << encoding_;
    if (rtAudio_->isStreamOpen()) rtAudio_->closeStream();
    rtAudio_->openStream(&parameters_, NULL, RTAUDIO_SINT16,
        sampleRate_, &bufferFrames_, &playCb, (void*) this);
    rtAudio_->startStream();
}

int AudioStreamMp3::playCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, 
    double streamTime, RtAudioStreamStatus status)
{
    BOOST_LOG_TRIVIAL(debug) << "nBufferFrames=" << nBufferFrames << ", streamTime=" << streamTime;
    if ((nPlayedFrames_ * 2) % mp3DecoderOutputBufferSize_ == 0)
    {
        nPlayedFrames_ = 0;
        int mpg123readResult = mpg123_read(mh_, mp3DecoderOutputBuffer_, mp3DecoderOutputBufferSize_, &nDecodedBytesToProcessLeft_);
        BOOST_LOG_TRIVIAL(debug) << "Bytes decoded: " << nDecodedBytesToProcessLeft_
            << ". End of mp3 decoding -> Mpg123 read result: " << mpg123readResult;
        if (mpg123readResult != MPG123_OK)
        {
            doneDecodingMp3_ = true;
        }
    }
    
    int16_t* outBuffer = static_cast<int16_t*>(outputBuffer);
    int16_t* mp3DecoderOutputBuffer = reinterpret_cast<int16_t*>(mp3DecoderOutputBuffer_);

    for (int i = 0; i < nBufferFrames; ++i)
    {
        *outBuffer++ = *(mp3DecoderOutputBuffer++ + nPlayedFrames_);
        nDecodedBytesToProcessLeft_ -= 2;
        if (nDecodedBytesToProcessLeft_ == 0) break;
    }
    nPlayedFrames_ += nBufferFrames;
    
    BOOST_LOG_TRIVIAL(debug) << "Total played frames: " << nPlayedFrames_ 
        << ". Decoder output buffer size: " << mp3DecoderOutputBufferSize_
        << ". nDecodedBytesToProcessLeft: " << nDecodedBytesToProcessLeft_;

    if (doneDecodingMp3_ && nDecodedBytesToProcessLeft_ == 0)
    {
        BOOST_LOG_TRIVIAL(info) << "Finished playing mp3 audio stream." << rtAudio_->isStreamOpen() << " " << rtAudio_->isStreamRunning();
        mpg123_close(mh_);
        doneDecodingMp3_ = false;
        BOOST_LOG_TRIVIAL(info) << "Closed audio stream successfully.";
        audioStreamPromise_.set_value(this);
        return 1;
    }

    return 0;
}

}