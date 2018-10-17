#include "PDBClient.h"

void PDBClient::connectToServer(const std::string& ip, const std::string& port)
{
    pdbNetwork_.connectToServer(ip, port);
}