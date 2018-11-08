#ifndef _PDB_SERVER_
#define _PDB_SERVER_
#include "PDBNetwork.h"

class PDBServer
{
public:
    void init();
    void run();
private:
    PDBNetwork pdbNetwork_;
};

#endif