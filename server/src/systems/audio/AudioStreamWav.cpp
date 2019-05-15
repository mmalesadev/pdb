#include "AudioStreamWav.h"

namespace Pdb
{

AudioStreamWav::AudioStreamWav(std::atomic<float>& masterVolume) : AudioStream(masterVolume)
{

}

void AudioStreamWav::play()
{
    volume_ = playedAudioTrack_->getVolume();

    std::string path = playedAudioTrack_->getFilePath();
    audioFile_.load(path);
    parameters_.nChannels = audioFile_.getNumChannels();
    parameters_.firstChannel = 0;
    sampleRate_ = audioFile_.getSampleRate();
    bufferFrames_ = 256;

    if (rtAudio_->isStreamOpen()) rtAudio_->closeStream();
    rtAudio_->openStream(&parameters_, NULL, RTAUDIO_FLOAT64,
        sampleRate_, &bufferFrames_, &playCb, (void *) this);
    rtAudio_->startStream();
}

void AudioStreamWav::play(const AudioTrack& audioTrack)
{
    volume_ = audioTrack.getVolume();

    std::string path = audioTrack.getFilePath();
    audioFile_.load(path);
    parameters_.nChannels = audioFile_.getNumChannels();
    parameters_.firstChannel = 0;
    sampleRate_ = audioFile_.getSampleRate();
    bufferFrames_ = 256;

    if (rtAudio_->isStreamOpen()) rtAudio_->closeStream();
    rtAudio_->openStream(&parameters_, NULL, RTAUDIO_FLOAT64,
        sampleRate_, &bufferFrames_, &playCb, (void *) this);
    rtAudio_->startStream();
}

void AudioStreamWav::stop()
{
}

int AudioStreamWav::playCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status)
{
    double* outBuffer = static_cast<double*>(outputBuffer);
    if(nProcessedSamples_ >= audioFile_.getNumSamplesPerChannel())
    {
        return 1;
    }
    for(int i = 0; i < nBufferFrames; ++i)
    {
        for(int j = 0; j < audioFile_.getNumChannels(); ++j)
        {
            *outBuffer++ = audioFile_.samples[j][nProcessedSamples_ + i];
        }
    }
    nProcessedSamples_ += nBufferFrames;

    return 0;
}

void AudioStreamWav::seek(int offsetInSeconds)
{
}

}