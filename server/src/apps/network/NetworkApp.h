#pragma once
#include "apps/App.h"

namespace Pdb
{

class NetworkApp : public App
{
public:
    NetworkApp(VoiceManager& voiceManager);

    void init();
    void appLoopFunction();
};

}