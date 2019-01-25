#ifndef _PDB_APP_
#define _PDB_APP_
#include <thread>

class PDBApp
{
public:
    virtual ~PDBApp();
    void start();
    
private:
    virtual void init() = 0;
    virtual void appLoopFunction() = 0;

private:
    std::thread thread_;
};

#endif