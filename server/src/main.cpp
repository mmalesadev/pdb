#include "Server.h"

#include "apps/network/NetworkApp.h"
#include "apps/audiobook/AudiobookApp.h"

#include <memory>

int main(int argc, char* argv[])
{
    Pdb::Server server;

    server.registerApp("network", std::make_unique<Pdb::NetworkApp>());
    server.registerApp("audiobook", std::make_unique<Pdb::AudiobookApp>(server.getAudioManager()));

    server.run();
    
    return 0;
}
