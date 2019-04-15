#pragma once
#include "systems/audio/AudioTrack.h"
#include "systems/audio/AudioManager.h"
#include "systems/voice/VoiceManager.h"
#include "systems/input/InputManager.h"
#include <atomic>
#include <thread>

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
    void rewind();
    void fastForward();
    void stopAudiobook();

    void printState();

    std::vector< std::pair<InputManager::Button, std::function<void()> > > & getAvailableActions();

    AudioTrack getCurrentTrack()                        { return audioTracks_[currentTrackIndex_]; }
    std::vector<AudioTrack> & getAudioTracks()          { return audioTracks_; }

private:
    void loadTracks();
    void playAudiobook(AudioTrack& audioTrack);

    void fastForwardingTimerFunction();

    AudioManager& audioManager_;
    VoiceManager& voiceManager_;

    int currentTrackIndex_;
    std::vector<AudioTrack> audioTracks_;
    AudioStream* currentlyPlayedAudioStream_;

    int fastForwardingSpeed_;
    std::atomic<int> fastForwardedSeconds_;
    std::thread fastForwardingTimerThread_;

    std::unordered_map<State, std::vector< std::pair<InputManager::Button, std::function<void()> > > > availableActions_;

    State currentState_;
    void changeStateTo(State destinationState)          { currentState_ = destinationState; }
};

}