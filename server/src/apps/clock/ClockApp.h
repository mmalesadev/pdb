#pragma once
#include "apps/App.h"

namespace Pdb
{

class ClockApp : public App
{
public:
    ClockApp(VoiceManager& voiceManager);
    void init() override;    
    void appLoopFunction() override;

    void playCurrentTime();
    void playCurrentDate();

private:
    void synthesizeClockReadings();

    AudioTask * currentAudioTask_;
};

}