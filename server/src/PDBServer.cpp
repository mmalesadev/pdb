#include "PDBServer.h"
#include <thread>
#include <chrono>

void PDBServer::init()
{
    apps_.insert(std::make_pair(PdbApps::PDB_NETWORK, std::make_unique<PDBNetwork>()));
    apps_.insert(std::make_pair(PdbApps::PDB_AUDIOBOOK, std::make_unique<PDBAudiobook>(audioManager_)));

    for (auto& app : apps_)
    {
        app.second->start();
    }
}

void PDBServer::run()
{
    //inputManager_.handlePressedKeys();
    while(true);
}
