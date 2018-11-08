#ifndef _PDB_NETWORK_
#define _PDB_NETWORK_
#include <string>

class PDBNetwork
{

public:

    void connectToServer(const std::string& ip, const std::string& port);

};

#endif