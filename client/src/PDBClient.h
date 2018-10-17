#ifndef _PDB_CLIENT_
#define _PDB_CLIENT_
#include "PDBNetwork.h"

class PDBClient
{
public:
    void connectToServer(const std::string& ip, const std::string& port);
    
private:
    PDBNetwork pdbNetwork_;
};

#endif