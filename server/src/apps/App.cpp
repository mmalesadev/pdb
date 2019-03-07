#include "App.h"

namespace Pdb
{

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