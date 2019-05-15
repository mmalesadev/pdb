#pragma once
#include "systems/audio/AudioTrack.h"
#include "systems/audio/AudioManager.h"
#include "systems/voice/VoiceManager.h"
#include "systems/input/InputManager.h"
#include <atomic>
#include <thread>
#include <mutex>

namespace Pdb
{

class AudiobookPlayer
{
public:
    enum class State {CHOOSING, PLAYING, REWINDING, FAST_FORWARDING, PAUSED};
    
    AudiobookPlayer(AudioManager& audioManager, VoiceManager& voiceManager);

    void switchToNextAudiobook();
    void switchToPreviousAudiobook();
    void playChosenAudiobook();
    void pauseToggle();
    void rewind();
    void fastForward();
    void stopAudiobook();

    void setCurrentAudioTask(AudioTask* audioTask) { currentAudioTask_ = audioTask; }
    void printState();

    std::vector< std::pair<InputManager::Button, std::function<void()> > > & getAvailableActions();

    AudioTrack& getCurrentTrack()                        { return audioTracks_[currentTrackIndex_]; }
    std::vector<AudioTrack> & getAudioTracks()          { return audioTracks_; }

private:
    void loadTracks();
    void play(std::list<AudioTask::Element> audioTaskElements, std::function<void()> callbackFunction = {});

    void fastForwardingTimerFunction();

    AudioManager& audioManager_;
    VoiceManager& voiceManager_;

    int currentTrackIndex_;
    std::vector<AudioTrack> audioTracks_;
    AudioTask* currentAudioTask_;
    AudioTask* pausedAudioTask_;

    int fastForwardingSpeed_;
    std::atomic<int> fastForwardedSeconds_;
    std::thread fastForwardingTimerThread_;

    std::unordered_map<State, std::vector< std::pair<InputManager::Button, std::function<void()> > > > availableActions_;

    State currentState_;
    void changeStateTo(State destinationState)          { currentState_ = destinationState; }

    std::mutex mutex_;
};

}