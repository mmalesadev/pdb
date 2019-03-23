#pragma once
#include "systems/audio/AudioTrack.h"
#include "systems/audio/AudioManager.h"
#include "systems/voice/VoiceManager.h"

namespace Pdb
{

class AudiobookPlayer
{
public:
    AudiobookPlayer(AudioManager& audioManager, VoiceManager& voiceManager);
    void playAudiobook(AudioTrack& audioTrack);
    void pauseAudiobook();

private:
    AudioManager& audioManager_;
    VoiceManager& voiceManager_;

    std::future<AudioStream*> currentlyPlayedAudioStream_;
};

}