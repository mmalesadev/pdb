#pragma once
#include "apps/App.h"

namespace Pdb
{

class NetworkApp : public App
{
public:
    void init();
    void appLoopFunction();
};

}