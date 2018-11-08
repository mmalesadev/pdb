#include "PDBNetwork.h"
#include <iostream>
#include <boost/asio.hpp>

void PDBNetwork::connectToServer(const std::string& ip, const std::string& port)
{
    using namespace boost::asio::ip;

    try
    {
       boost::asio::io_context io_context;

       tcp::resolver resolver(io_context);
       tcp::resolver::results_type endpoints = resolver.resolve(ip, port);

       tcp::socket socket(io_context);
       boost::asio::connect(socket, endpoints);
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}