#include "AudioPlayer.h"

AudioPlayer::AudioPlayer()
{

}

AudioPlayer::~AudioPlayer()
{

}

void AudioPlayer::run(AudioTrack & track)
{
    init();
    open(track);
    getDecodingFormat();
    setOutputFormat();
    openOutputDevice();
    play();
    destroy();
}

void AudioPlayer::init()
{
    ao_initialize();
    driver_ = ao_default_driver_id();
    mpg123_init();
    mh_ = mpg123_new(NULL, &err_);
    buffer_size_ = mpg123_outblock(mh_);
    buffer_ = (unsigned char *) malloc(buffer_size_ * sizeof(unsigned char));
}

void AudioPlayer::open(AudioTrack & track)
{
    std::string path = "../data/" + track.getTitle();
    mpg123_open(mh_, path.c_str());
}

void AudioPlayer::getDecodingFormat()
{
    mpg123_getformat(mh_, &rate_, &channels_, &encoding_);
}

void AudioPlayer::setOutputFormat()
{
    format_.bits = mpg123_encsize(encoding_) * BITS;
    format_.rate = rate_;
    format_.channels = channels_;
    format_.byte_format = AO_FMT_NATIVE;
    format_.matrix = 0;
}

void AudioPlayer::openOutputDevice()
{
    dev_ = ao_open_live(driver_, &format_, NULL);
}

void AudioPlayer::play()
{
    while (mpg123_read(mh_, buffer_, buffer_size_, &done_) == MPG123_OK)
        ao_play(dev_, (char *)buffer_, done_);
}

void AudioPlayer::destroy()
{
    free(buffer_);
    ao_close(dev_);
    mpg123_close(mh_);
    mpg123_delete(mh_);
    mpg123_exit();
    ao_shutdown();
}