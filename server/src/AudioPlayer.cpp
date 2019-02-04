#include "AudioPlayer.h"
#include <cstring>

AudioPlayer::AudioPlayer()
{
}

AudioPlayer::~AudioPlayer()
{
    destroy();
}

int saw(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData)
{
  unsigned int i, j;
  double *buffer = (double *) outputBuffer;
  double *lastValues = (double *) userData;
  if ( status )
    std::cout << "Stream underflow detected!" << std::endl;
  // Write interleaved audio data.
  for ( i=0; i<nBufferFrames; i++ ) {
    for ( j=0; j<2; j++ ) {
      *buffer++ = lastValues[j];
      lastValues[j] += 0.005 * (j+1+(j*0.1));
      if ( lastValues[j] >= 1.0 ) lastValues[j] -= 2.0;
    }
  }
  return 0;
}

int mp3Cb(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData)
{
    return ((AudioPlayer*)userData)->mp3Callback(outputBuffer, inputBuffer, nBufferFrames, streamTime, status);
}

int AudioPlayer::mp3Callback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status)
{
    if (status)
        std::cout << "Stream underflow detected!" << std::endl;
    if ((nPlayedFrames_ * 2) % mp3DecoderOutputBufferSize == 0)
    {
        nPlayedFrames_ = 0;
        if (mpg123_read(mh_, mp3DecoderOutputBuffer_, mp3DecoderOutputBufferSize, &done_) != MPG123_OK)
            return 0;
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

void AudioPlayer::playMP3(AudioTrack & audioTrack)
{
    try {
        rtAudio_->openStream(&parameters_, NULL, RTAUDIO_SINT16,
                    sampleRate_, &bufferFrames_, &mp3Cb, (void*) this);
        rtAudio_->startStream();
    }
    catch (RtAudioError& e) {
        e.printMessage();
        exit(0);
    }
}

void AudioPlayer::playWAV(AudioTrack & audioTrack)
{
    double data[2];
    try {
        rtAudio_->openStream(&parameters_, NULL, RTAUDIO_FLOAT64,
                    sampleRate_, &bufferFrames_, &saw, (void *)&data);
        rtAudio_->startStream();
    }
    catch (RtAudioError& e) {
        e.printMessage();
        exit(0);
    }
}

void AudioPlayer::initMP3(AudioTrack & audioTrack)
{
    mpg123_init();
    mh_ = mpg123_new(NULL, &err_);
    mp3DecoderOutputBufferSize = mpg123_outblock(mh_);
    std::cout << "mpg123 buffer size = " << mp3DecoderOutputBufferSize << " bytes\n";
    mp3DecoderOutputBuffer_ = (unsigned char *) malloc(mp3DecoderOutputBufferSize * sizeof(unsigned char));
    std::string path = "../data/" + audioTrack.getTitle();
    mpg123_open(mh_, path.c_str());
    mpg123_getformat(mh_, &rate_, &channels_, &encoding_);
    nPlayedFrames_ = 0;
    std::cout << "Mp3 rate = " << rate_ << ", channels = " << channels_ << ", encoding = " << encoding_ <<
        ", encsize = " << mpg123_encsize(encoding_) << "\n";
    
    rtAudio_ = std::make_unique<RtAudio>();
    if (rtAudio_->getDeviceCount() < 1) {
        std::cout << "No audio devices found!\n";
        exit(0);
    }
    parameters_.deviceId = rtAudio_->getDefaultOutputDevice();
    parameters_.nChannels = channels_;
    parameters_.firstChannel = 0;
    sampleRate_ = rate_;
    bufferFrames_ = 256;
}

void AudioPlayer::initWAV(AudioTrack & audioTrack)
{
    rtAudio_ = std::make_unique<RtAudio>();
    if (rtAudio_->getDeviceCount() < 1) {
        std::cout << "No audio devices found!\n";
        exit(0);
    }
    parameters_.deviceId = rtAudio_->getDefaultOutputDevice();
    parameters_.nChannels = 2;
    parameters_.firstChannel = 0;
    sampleRate_ = 44100;
    bufferFrames_ = 256;
}

void AudioPlayer::destroy()
{
    free(mp3DecoderOutputBuffer_);
    mpg123_close(mh_);
    mpg123_delete(mh_);
    mpg123_exit();
}