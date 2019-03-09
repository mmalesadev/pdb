#pragma once

#include "systems/audio/AudioManager.h"
#include "systems/input/InputManager.h"
#include <thread>

namespace Pdb
{

class App
{
public:
    App(AudioManager& audioManager, InputManager& inputManager);
    virtual ~App();
    void start();
    
private:
    virtual void init() = 0;
    virtual void appLoopFunction() = 0;

    std::thread thread_;
    
protected:
    AudioManager& audioManager_;
    InputManager& inputManager_;
};

}