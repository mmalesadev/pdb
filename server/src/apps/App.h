#pragma once

#include "systems/audio/AudioManager.h"
#include "systems/input/InputManager.h"
#include "systems/voice/VoiceManager.h"
#include <thread>
#include <string>

namespace Pdb
{

class App
{
public:
    App(VoiceManager& voiceManager);
    virtual ~App();
    void start();
    void setName(const std::string & name) { name_ = name; }
    
private:
    virtual void init() = 0;
    virtual void appLoopFunction() = 0;

    std::thread thread_;
    std::string name_;
    
protected:
    AudioManager audioManager_;
    InputManager inputManager_;
    VoiceManager& voiceManager_;
};

}