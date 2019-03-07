#pragma once
#include <unordered_map>
#include <memory>
#include <string>

#include "apps/App.h"
#include "systems/input/InputManager.h"
#include "systems/audio/AudioManager.h"

namespace Pdb
{

class Server
{
public:
    void registerApp(std::string name, std::unique_ptr<App> App);
    void run();

    InputManager& getInputManager() { return inputManager_; }
    AudioManager& getAudioManager() { return audioManager_; }

private:
    std::unordered_map< std::string, std::unique_ptr<App> > apps_;

    InputManager inputManager_;
    AudioManager audioManager_;
};

}