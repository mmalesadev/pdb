#include "PDBNetwork.h"
#include <iostream>
#include <boost/log/trivial.hpp>

void PDBNetwork::init()
{
    BOOST_LOG_TRIVIAL(info) << "Initialized PDBNetwork";
}

void PDBNetwork::appLoopFunction()
{
    BOOST_LOG_TRIVIAL(info) << "Starting PDBNetwork loop function.";

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
        BOOST_LOG_TRIVIAL(error) << e.what();
    }

    BOOST_LOG_TRIVIAL(info) << "Ending PDBNetwork loop function.";
}
