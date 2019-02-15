#include "PDBServer.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

int main(int argc, char* argv[])
{
    auto logger = spdlog::stdout_color_mt("console");

    PDBServer pdbServer;
    pdbServer.init();
    pdbServer.run();
    
    return 0;
}
