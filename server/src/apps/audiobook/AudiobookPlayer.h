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

    void switchToNextAudiobook();
    void switchToPreviousAudiobook();
    void playCurrentTrack();
    void pauseAudiobook();

    AudioTrack getCurrentTrack()                        { return audioTracks_[currentIndex_]; }
    std::vector<AudioTrack> & getAudioTracks()          { return audioTracks_; }

private:
    AudioManager& audioManager_;
    VoiceManager& voiceManager_;

    void loadTracks();
    void playAudiobook(AudioTrack& audioTrack);
    int currentIndex_;

    std::vector<AudioTrack> audioTracks_;

    std::future<AudioStream*> currentlyPlayedAudioStream_;
};

}