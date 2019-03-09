#include "Server.h"

#include "apps/network/NetworkApp.h"
#include "apps/audiobook/AudiobookApp.h"

#include <memory>

int main(int argc, char* argv[])
{
    Pdb::Server server;

    server.registerApp("network", std::make_unique<Pdb::NetworkApp>(server.getAudioManager(), server.getInputManager()));
    server.registerApp("audiobook", std::make_unique<Pdb::AudiobookApp>(server.getAudioManager(), server.getInputManager()));

    server.run();
    
    return 0;
}
