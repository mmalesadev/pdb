#include "AudiobookPlayer.h"
#include <boost/log/trivial.hpp>

namespace Pdb
{

AudiobookPlayer::AudiobookPlayer(AudioManager& audioManager) : audioManager_(audioManager), currentlyPlayedAudioStream_(nullptr)
{
    
}

void AudiobookPlayer::playAudiobook(AudioTrack& audioTrack)
{
    if(!currentlyPlayedAudioStream_)
    {
        currentlyPlayedAudioStream_ = audioManager_.playAndGetAudioStream(audioTrack);
        audioManager_.playAndGetAudioStream(audioTrack);
        audioManager_.playAndGetAudioStream(audioTrack);
    }
    else
    {
        BOOST_LOG_TRIVIAL(error) << "Cannot play sound. No available streams left.";
    }
}

void AudiobookPlayer::pauseAudiobook()
{
    
}

}