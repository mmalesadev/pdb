#pragma once
#include <unordered_map>
#include <memory>
#include <string>

#include "apps/App.h"
#include "systems/input/InputManager.h"
#include "systems/audio/AudioManager.h"
#include "systems/voice/VoiceManager.h"

namespace Pdb
{

class Server
{
public:
    void registerApp(const std::string& name, std::unique_ptr<App> app);
    void run();

    VoiceManager& getVoiceManager() { return voiceManager_; }

private:
    std::unordered_map< std::string, std::unique_ptr<App> > apps_;

    VoiceManager voiceManager_;
};

}