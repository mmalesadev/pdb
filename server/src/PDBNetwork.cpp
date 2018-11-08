#include "PDBNetwork.h"
#include <iostream>
#include <boost/asio.hpp>

PDBNetwork::PDBNetwork()
{
}

PDBNetwork::~PDBNetwork()
{
    if (thread_.joinable()) thread_.join();
}

void PDBNetwork::init()
{
    std::thread thread(&PDBNetwork::run, this);
    thread_ = std::move(thread);
}

void PDBNetwork::run()
{
    using namespace boost::asio::ip;

    try
    {
        boost::asio::io_context io_context; 
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 7171));

        tcp::socket socket(io_context);
        acceptor.accept(socket);
        std::cout << "Client connected\n";

        // boost::system::error_code ignored_error;
        // boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
        
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}