#include "Server.h"
#include <thread>
#include <chrono>

namespace Pdb
{

void Server::registerApp(std::string name, std::unique_ptr<App> App)
{
    apps_.insert(std::make_pair(name, std::move(App)));
}

void Server::run()
{
    /* Firstly we start all registered applications (threads) */
    for (auto& app : apps_) app.second->start();

    /* And then, for now, main loops infinitely waits here */
    while(true);
}

}