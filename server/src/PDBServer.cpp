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
        std::cout << "Main thread run() function log" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }
}
