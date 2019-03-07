#pragma once
#include "systems/audio/AudioTrack.h"
#include "systems/audio/AudioManager.h"

namespace Pdb
{

class AudiobookPlayer
{
public:
    AudiobookPlayer(AudioManager& audioManager);
    void playAudiobook(AudioTrack& audioTrack);
    void pauseAudiobook();

    AudioStream* getCurrentlyPlayedAudioStream() const { return currentlyPlayedAudioStream_; }

private:
    AudioManager& audioManager_;

    AudioStream* currentlyPlayedAudioStream_;
};

}