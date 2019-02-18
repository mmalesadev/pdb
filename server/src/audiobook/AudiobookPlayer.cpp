#include "AudiobookPlayer.h"
#include "spdlog/spdlog.h"

AudiobookPlayer::AudiobookPlayer(AudioManager& audioManager) : audioManager_(audioManager), currentlyPlayedAudioStream_(nullptr)
{
    
}

void AudiobookPlayer::playAudiobook(AudioTrack& audioTrack)
{
    // TODO: look at std::optional instead of raw pointer
    if(!currentlyPlayedAudioStream_)
    {
        currentlyPlayedAudioStream_ = audioManager_.playAndGetAudioStream(audioTrack);
        audioManager_.playAndGetAudioStream(audioTrack);
        audioManager_.playAndGetAudioStream(audioTrack);
    }
    else
    {
        spdlog::get("console")->error("Cannot play sound. No available streams left.");
    }
}

void AudiobookPlayer::pauseAudiobook()
{
    
}