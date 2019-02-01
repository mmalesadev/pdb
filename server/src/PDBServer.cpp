#include "PDBServer.h"
#include <thread>
#include <iostream>
#include <chrono>

void PDBServer::init()
{
    apps_.insert(std::make_pair(PdbApps::PDB_NETWORK, std::make_unique<PDBNetwork>()));
    apps_.insert(std::make_pair(PdbApps::PDB_AUDIOBOOK, std::make_unique<PDBAudiobook>()));

    for (auto& app : apps_)
    {
        app.second->start();
    }
}

void PDBServer::run()
{
    AudioPlayer audioPlayer;
    AudioTrack track("powrot-z-gwiazd.mp3");
    
    //inputManager_.handlePressedKeys();
    audioPlayer.playWAV(track);
    while(true);

}
