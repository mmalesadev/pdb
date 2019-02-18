#include "PDBServer.h"
#include <boost/log/trivial.hpp>

int main(int argc, char* argv[])
{
    PDBServer pdbServer;
    pdbServer.init();
    pdbServer.run();
    
    return 0;
}
