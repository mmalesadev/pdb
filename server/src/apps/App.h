#pragma once
#include <thread>

namespace Pdb
{

class App
{
public:
    virtual ~App();
    void start();
    
private:
    virtual void init() = 0;
    virtual void appLoopFunction() = 0;

private:
    std::thread thread_;
};

}