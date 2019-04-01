#pragma once
#include "apps/App.h"
#include "apps/audiobook/AudiobookPlayer.h"
#include <cstring>
#include <vector>
#include "Config.h"

namespace Pdb
{

class AudiobookApp : public App
{
public:
    AudiobookApp(AudioManager& audioManager, InputManager& inputManager, VoiceManager& voiceManager);
    void init();
    void appLoopFunction();

private:
    void synthesizeVoiceMessages();
    
    AudiobookPlayer audiobookPlayer_;
};

}