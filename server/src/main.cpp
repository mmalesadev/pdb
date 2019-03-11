#include "Server.h"
#include "Config.h"

#include "apps/network/NetworkApp.h"
#include "apps/audiobook/AudiobookApp.h"

#include <memory>

int main(int argc, char* argv[])
{
    Pdb::Config::getInstance();
    Pdb::Server server;

    server.registerApp("network", std::make_unique<Pdb::NetworkApp>(server.getAudioManager(), server.getInputManager(), server.getVoiceManager()));
    server.registerApp("audiobook", std::make_unique<Pdb::AudiobookApp>(server.getAudioManager(), server.getInputManager(), server.getVoiceManager()));

    server.run();
    
    return 0;
}

