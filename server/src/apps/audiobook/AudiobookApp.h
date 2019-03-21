#pragma once
#include "apps/App.h"
#include "apps/audiobook/AudiobookPlayer.h"
#include <cstring>
#include <vector>

namespace Pdb
{

class AudiobookApp : public App
{
public:
    AudiobookApp(AudioManager& audioManager, InputManager& inputManager, VoiceManager& voiceManager);
    void init();
    void appLoopFunction();

private:
    void loadTracks();
    void synthesizeTrackTitles();

    std::vector<AudioTrack> audioTracks_;

    AudiobookPlayer audiobookPlayer_;
};

}