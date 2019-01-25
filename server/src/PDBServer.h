#ifndef _PDB_SERVER_
#define _PDB_SERVER_
#include <unordered_map>
#include <memory>
#include "PDBApp.h"
#include "PDBNetwork.h"
#include "audiobook/PDBAudiobook.h"
#include "InputManager.h"

class PDBServer
{
public:
    void init();
    void run();
private:
    enum class PdbApps { PDB_NETWORK, PDB_AUDIOBOOK };
    std::unordered_map<PdbApps, std::unique_ptr<PDBApp>> apps_;
    InputManager inputManager_;
};

#endif