#pragma once
#include "apps/App.h"
#include "apps/audiobook/AudiobookPlayer.h"
#include <cstring>
#include <vector>
#include "Config.h"

namespace Pdb
{

enum States {CHOOSING, PLAYING, REWINDING, REWINDING_FORWARD, PAUSE};

class AudiobookApp : public App
{
public:
    AudiobookApp(AudioManager& audioManager, InputManager& inputManager, VoiceManager& voiceManager);
    void init();
    void appLoopFunction();
    void changeStateTo(States state);

private:
    void synthesizeVoiceMessages();
    
    States currentState_;
    

    AudiobookPlayer audiobookPlayer_;
};

}