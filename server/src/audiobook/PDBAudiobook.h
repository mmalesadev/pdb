#ifndef _PDB_AUDIOBOOK_
#define _PDB_AUDIOBOOK_
#include "PDBApp.h"
#include <cstring>

class PDBAudiobook : public PDBApp
{
public:
    void init();
    void appLoopFunction();
private:
    void playMessage(const std::string & message);
};

#endif