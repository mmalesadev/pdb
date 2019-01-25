#include "PDBNetwork.h"
#include <iostream>
#include <boost/asio.hpp>

void PDBNetwork::init()
{
    std::cout << "Initialized PDBNetwork.\n";
}

void PDBNetwork::appLoopFunction()
{
    using namespace boost::asio::ip;

    std::cout << "Starting PDBNetwork loop function.\n";

    try
    {
	//     boost::asio::io_context io_context; 
    //     tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 7171));

    //     tcp::socket socket(io_context);
    //     acceptor.accept(socket);
    //     std::cout << "Client connected\n";

    //     boost::system::error_code ignored_error;
    //     boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
        
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    std::cout << "Ending PDBNetwork loop function.\n";
}
