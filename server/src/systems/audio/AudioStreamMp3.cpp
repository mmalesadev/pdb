#include "AudioStreamMp3.h"

namespace Pdb
{

AudioStreamMp3::AudioStreamMp3(std::atomic<float>& masterVolume) : AudioStream(masterVolume)
{
    mpg123_init();
    int err;
    mh_ = mpg123_new(NULL, &err);
    mp3DecoderOutputBufferSize_ = mpg123_outblock(mh_);
    mp3DecoderOutputBuffer_ = (unsigned char *) malloc(mp3DecoderOutputBufferSize_ * sizeof(unsigned char));
    doneDecodingMp3_= false;
}

AudioStreamMp3::~AudioStreamMp3()
{
    free(mp3DecoderOutputBuffer_);
    mpg123_close(mh_);
    mpg123_delete(mh_);
    mpg123_exit();
}

void AudioStreamMp3::play()
{
    state_ = State::PLAYING;
    volume_ = playedAudioTrack_->getVolume();

    std::string path = playedAudioTrack_->getFilePath();
    mpg123_open(mh_, path.c_str());
    mpg123_getformat(mh_, &rate_, &channels_, &encoding_);
    nPlayedFrames_ = 0;
    
    parameters_.nChannels = channels_;
    parameters_.firstChannel = 0;
    sampleRate_ = rate_;
    bufferFrames_ = 256;
    doneDecodingMp3_ = false;

    BOOST_LOG_TRIVIAL(info) << "Playing mp3 audio stream. Rate: " << rate_ << ", channels: " << channels_ << ", encoding: " << encoding_;
    BOOST_LOG_TRIVIAL(info) << "Audio track played: " << playedAudioTrack_->getTrackName();
    if (rtAudio_->isStreamOpen()) rtAudio_->closeStream();
    rtAudio_->openStream(&parameters_, NULL, RTAUDIO_SINT16,
        sampleRate_, &bufferFrames_, &playCb, (void*) this);
    if (playedAudioTrack_->getLastPlayedMillisecond() > 0)
        seek(playedAudioTrack_->getLastPlayedMillisecond());
    rtAudio_->startStream();
}

void AudioStreamMp3::stop()
{
    std::unique_lock<std::mutex> lock(mutex_);

    BOOST_LOG_TRIVIAL(info) << "Stopping mp3 audio stream. open: " << rtAudio_->isStreamOpen() << ", running: " << rtAudio_->isStreamRunning();
    rtAudio_->closeStream();
    mpg123_close(mh_);
    doneDecodingMp3_ = false;
    state_ = State::AVAILABLE;
    finishedPlayingCondVar_.notify_all();
    BOOST_LOG_TRIVIAL(info) << "Stopped mp3 audio stream. open: " << rtAudio_->isStreamOpen() << ", running: " << rtAudio_->isStreamRunning();
}

void AudioStreamMp3::play(const AudioTrack& audioTrack)
{
    // TODO: Funkcja do wyrzucenia
    std::unique_lock<std::mutex> lock(mutex_);
    
    volume_ = audioTrack.getVolume();

    std::string path = audioTrack.getFilePath();
    mpg123_open(mh_, path.c_str());
    mpg123_getformat(mh_, &rate_, &channels_, &encoding_);
    nPlayedFrames_ = 0;
    
    parameters_.nChannels = channels_;
    parameters_.firstChannel = 0;
    sampleRate_ = rate_;
    bufferFrames_ = 256;
    doneDecodingMp3_ = false;

    BOOST_LOG_TRIVIAL(info) << "Playing mp3 audio stream. Rate: " << rate_ << ", channels: " << channels_ << ", encoding: " << encoding_;
    if (rtAudio_->isStreamOpen()) rtAudio_->closeStream();
    rtAudio_->openStream(&parameters_, NULL, RTAUDIO_SINT16,
        sampleRate_, &bufferFrames_, &playCb, (void*) this);

    rtAudio_->startStream();
    if (audioTrack.getLastPlayedMillisecond() > 0)
        seek(audioTrack.getLastPlayedMillisecond());
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

    BOOST_LOG_TRIVIAL(debug) << "Current sample pos: " << mpg123_tell(mh_) << ", Current frame: " << mpg123_tellframe(mh_) << ", byte offset: " << mpg123_tell_stream(mh_);

    for (int i = 0; i < nBufferFrames; ++i)
    {
        *outBuffer = *(mp3DecoderOutputBuffer + nPlayedFrames_) * volume_ * masterVolume_;

        mp3DecoderOutputBuffer++;
        nDecodedBytesToProcessLeft_ -= 2;
        outBuffer++;

        if (nDecodedBytesToProcessLeft_ <= 0) break;
    }
    nPlayedFrames_ += nBufferFrames;
    
    BOOST_LOG_TRIVIAL(debug) << "Total played frames: " << nPlayedFrames_ 
        << ". Decoder output buffer size: " << mp3DecoderOutputBufferSize_
        << ". nDecodedBytesToProcessLeft: " << nDecodedBytesToProcessLeft_;

    if (doneDecodingMp3_ && nDecodedBytesToProcessLeft_ <= 0)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        BOOST_LOG_TRIVIAL(info) << "Finished playing mp3 audio stream." << rtAudio_->isStreamOpen() << " " << rtAudio_->isStreamRunning();
        mpg123_close(mh_);
        doneDecodingMp3_ = false;
        BOOST_LOG_TRIVIAL(info) << "Closed audio stream successfully.";
        state_ = State::AVAILABLE;
        finishedPlayingCondVar_.notify_all();
        return 1;
    }

    return 0;
}

int AudioStreamMp3::currentPositionInMilliseconds()
{
    return ((double)(mpg123_tell(mh_)) / (double)(rate_)) * 1000;
}

void AudioStreamMp3::seek(int offsetInMilliseconds)
{
    float secondsOffset = (float)(offsetInMilliseconds) / 1000.0f;
    off_t currentSample = mpg123_tell(mh_);
    off_t sampleOffset = secondsOffset * rate_;

    BOOST_LOG_TRIVIAL(info) << "Offset in seconds: " << secondsOffset << ". Current sample: " << currentSample << ". Changing stream position by: " << 
        sampleOffset << ". Set position to sample: " << currentSample + sampleOffset;

    mpg123_seek(mh_, currentSample + sampleOffset, SEEK_SET);
    nPlayedFrames_ = 0;
}


}