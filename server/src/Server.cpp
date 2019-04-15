#include "Server.h"
#include <thread>
#include <chrono>

namespace Pdb
{

void Server::registerApp(const std::string& name, std::unique_ptr<App> app)
{
    app->setName(name);
    apps_.insert(std::make_pair(name, std::move(app)));
}

void Server::run()
{
    /* Firstly we start all registered applications (threads) */
    for (auto& app : apps_) app.second->start();

    /* And then, for now, main loops infinitely waits here */
    while(true);
}

}