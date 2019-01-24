#include "PDBServer.h"
#include <thread>
#include <iostream>
#include <chrono>

void PDBServer::init()
{
    //pdbNetwork_.init();
}

void PDBServer::run()
{
    while(true)
    {
        inputManager_.handlePressedKeys();
    }
}
