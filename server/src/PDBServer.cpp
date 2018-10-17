#include "PDBServer.h"
#include <iostream>
#include <boost/asio.hpp>

void PDBServer::init()
{

}

void PDBServer::run()
{
    using namespace boost::asio::ip;

    try
    {
        boost::asio::io_context io_context; 
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 7171));
        while(true)
        {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::cout << "Client connected\n";

            // boost::system::error_code ignored_error;
            // boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
        }
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}