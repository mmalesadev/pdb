#pragma once
#include "systems/audio/AudioTrack.h"
#include "systems/audio/AudioManager.h"
#include "systems/voice/VoiceManager.h"
#include "systems/input/InputManager.h"

namespace Pdb
{

class AudiobookPlayer
{
public:
    enum class State {CHOOSING, PLAYING, REWINDING, FAST_FORWARDING, PAUSED};
    
    AudiobookPlayer(AudioManager& audioManager, VoiceManager& voiceManager);

    void switchToNextAudiobook();
    void switchToPreviousAudiobook();
    void playCurrentTrack();
    void togglePause();
    void increaseMasterVolume();
    void decreaseMasterVolume();
    void rewinding();
    void fastForwarding();
    void stopAudiobook();

    void printState();

    std::vector< std::pair<InputManager::Button, std::function<void()> > > & getAvailableActions();

    AudioTrack getCurrentTrack()                        { return audioTracks_[currentTrackIndex_]; }
    std::vector<AudioTrack> & getAudioTracks()          { return audioTracks_; }

private:
    AudioManager& audioManager_;
    VoiceManager& voiceManager_;

    void loadTracks();
    void playAudiobook(AudioTrack& audioTrack);
    void increaseRewindingSpeed();
    void decreaseRewindingSpeed();

    int currentTrackIndex_;
    std::vector<AudioTrack> audioTracks_;
    AudioStream* currentlyPlayedAudioStream_;

    std::unordered_map<State, std::vector< std::pair<InputManager::Button, std::function<void()> > > > availableActions_;

    State currentState_;
    void changeStateTo(State destinationState)          { currentState_ = destinationState; }
};

}