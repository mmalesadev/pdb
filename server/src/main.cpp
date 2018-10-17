#include "PDBServer.h"

int main(int argc, char* argv[])
{
    PDBServer pdbServer;
    pdbServer.init();
    pdbServer.run();

    return 0;
}