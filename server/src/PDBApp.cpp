#include "PDBApp.h"

PDBApp::~PDBApp()
{
    if (thread_.joinable()) thread_.join();
}

void PDBApp::start()
{
    init();
    thread_ = std::thread(&PDBApp::appLoopFunction, this);
}