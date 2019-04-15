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
    AudiobookApp(VoiceManager& voiceManager);
    void init() override;
    void appLoopFunction() override;

private:
    void synthesizeVoiceMessages();
    
    AudiobookPlayer audiobookPlayer_;
};

}