#include "App.h"

namespace Pdb
{

App::App(AudioManager& audioManager, InputManager& inputManager)
    : audioManager_(audioManager), inputManager_(inputManager)
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