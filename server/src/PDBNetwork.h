#ifndef _PDB_NETWORK_
#define _PDB_NETWORK_
#include <thread>

class PDBNetwork
{
public:
    PDBNetwork();
    virtual ~PDBNetwork();

    void init();
    void run();



private:
    std::thread thread_;
};

#endif