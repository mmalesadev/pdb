#include "AudioStreamWav.h"

AudioStreamWav::AudioStreamWav()
{

}

void AudioStreamWav::play(const AudioTrack& audioTrack)
{
    std::string path = "../data/" + audioTrack.getFileName();
    audioFile_.load(path);
    parameters_.nChannels = audioFile_.getNumChannels();
    parameters_.firstChannel = 0;
    sampleRate_ = audioFile_.getSampleRate();
    bufferFrames_ = 256;

    rtAudio_->openStream(&parameters_, NULL, RTAUDIO_FLOAT64,
        sampleRate_, &bufferFrames_, &playCb, (void *) this);
    rtAudio_->startStream();
}

int AudioStreamWav::playCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status)
{
    double* outBuffer = static_cast<double*>(outputBuffer);
    if(nProcessedSamples_ >= audioFile_.getNumSamplesPerChannel())
        return 1;
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