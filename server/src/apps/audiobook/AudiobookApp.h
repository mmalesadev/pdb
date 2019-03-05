#pragma once
#include "apps/App.h"
#include "systems/audio/AudioManager.h"
#include "apps/audiobook/AudiobookPlayer.h"
#include <cstring>

namespace Pdb
{

class AudiobookApp : public App
{
public:
    AudiobookApp(AudioManager& audioManager, InputManager& inputManager);
    void init();
    void appLoopFunction();
private:
    void playMessage(const std::string & message);

    AudiobookPlayer audiobookPlayer_;
};

}