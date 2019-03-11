#pragma once
#include "apps/App.h"
#include "apps/audiobook/AudiobookPlayer.h"
#include <cstring>
#include <dirent.h>
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
    std::vector<AudioTrack> audioTracks_;

    void loadTracks();

    AudiobookPlayer audiobookPlayer_;
};

}