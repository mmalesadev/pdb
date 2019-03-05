#pragma once

#include "systems/input/InputManager.h"
#include <thread>

namespace Pdb
{

class App
{
public:
    App(InputManager& inputManager);
    virtual ~App();
    void start();
    
private:
    virtual void init() = 0;
    virtual void appLoopFunction() = 0;

    std::thread thread_;
    
protected:
    InputManager& inputManager_;
};
