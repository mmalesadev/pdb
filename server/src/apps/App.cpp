#include "App.h"

namespace Pdb
{

App::App(AudioManager& audioManager, InputManager& inputManager, VoiceManager& voiceManager)
    : audioManager_(audioManager), inputManager_(inputManager), voiceManager_(voiceManager)
{

}

App::~App()
{
    if (thread_.joinable()) thread_.join();
}

void App::start()
{
    init();
    thread_ = std::thread(&App::appLoopFunction, this);
}

}