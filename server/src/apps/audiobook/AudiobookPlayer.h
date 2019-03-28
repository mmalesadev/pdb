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
    void pauseCurrentTrack();

    AudioTrack getCurrentTrack()                        { return audioTracks_[currentTrackIndex_]; }
    std::vector<AudioTrack> & getAudioTracks()          { return audioTracks_; }

private:
    AudioManager& audioManager_;
    VoiceManager& voiceManager_;

    void loadTracks();
    void playAudiobook(AudioTrack& audioTrack);

    int currentTrackIndex_;
    std::vector<AudioTrack> audioTracks_;
    AudioStream* currentlyPlayedAudioStream_;
};

}