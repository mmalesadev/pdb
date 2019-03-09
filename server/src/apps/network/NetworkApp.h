#pragma once
#include "apps/App.h"

namespace Pdb
{

class NetworkApp : public App
{
public:
    NetworkApp(AudioManager& audioManager, InputManager& inputManager);

    void init();
    void appLoopFunction();
};

}