#include "AudioPlayer.h"

AudioPlayer::AudioPlayer()
{

}

AudioPlayer::~AudioPlayer()
{

}

void AudioPlayer::run(AudioTrack & audioTrack)
{
    init();
    open(audioTrack);
    getDecodingFormat();
    play();
    destroy();
}

void AudioPlayer::init()
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

    mpg123_init();
    mh_ = mpg123_new(NULL, &err_);
    buffer_size_ = mpg123_outblock(mh_);
    buffer_ = (unsigned char *) malloc(buffer_size_ * sizeof(unsigned char));
}

void AudioPlayer::open(AudioTrack & audioTrack)
{
    std::string path = "../data/" + audioTrack.getTitle();
    mpg123_open(mh_, path.c_str());
}

void AudioPlayer::getDecodingFormat()
{
    mpg123_getformat(mh_, &rate_, &channels_, &encoding_);
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

void AudioPlayer::play()
{
    double data[2];
    try {
        rtAudio_->openStream(&parameters_, NULL, RTAUDIO_FLOAT64,
                    sampleRate_, &bufferFrames_, &saw, (void *)&data);
        rtAudio_->startStream();
    }
    catch (RtAudioError& e) {
        e.printMessage();
        exit( 0 );
    }
    while (mpg123_read(mh_, buffer_, buffer_size_, &done_) == MPG123_OK)
    {
        //ao_play(dev_, (char *)buffer_, done_);
    }
}

void AudioPlayer::destroy()
{
    free(buffer_);
    mpg123_close(mh_);
    mpg123_delete(mh_);
    mpg123_exit();
}