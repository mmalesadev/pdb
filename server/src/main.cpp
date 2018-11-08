#include "PDBServer.h"
#include <iostream>

int main(int argc, char* argv[])
{
    PDBServer pdbServer;
    pdbServer.init();
    pdbServer.run();

    return 0;
}