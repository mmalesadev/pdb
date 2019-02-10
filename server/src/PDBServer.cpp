#include "PDBServer.h"
#include <thread>
#include <iostream>
#include <chrono>
#include "AudioFile.h"

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

    AudioTrack track("klapsczang.wav");
    AudioPlayer audioPlayerWav, audioPlayerMp3;
    audioPlayerWav.initWAV(track);
    //audioPlayerMp3.initMP3(track);
    
    //inputManager_.handlePressedKeys();
    audioPlayerWav.playWAV(track);
    //audioPlayerMp3.playMP3(track);


    while(true);

}
