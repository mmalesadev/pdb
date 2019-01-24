#ifndef _PDB_SERVER_
#define _PDB_SERVER_
#include "PDBNetwork.h"
#include "audiobook/PDBAudiobook.h"
#include "InputManager.h"

class PDBServer
{
public:
    void init();
    void run();
private:
    PDBNetwork pdbNetwork_;
    PDBAudiobook pdbAudiobook_;

    InputManager inputManager_;
};

#endif