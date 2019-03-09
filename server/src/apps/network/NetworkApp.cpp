#include "NetworkApp.h"
#include <iostream>
#include <boost/log/trivial.hpp>

namespace Pdb
{

NetworkApp::NetworkApp(AudioManager& audioManager, InputManager& inputManager)
    : App(audioManager, inputManager)
{
    BOOST_LOG_TRIVIAL(info) << "Creating NetworkApp.";
}

void NetworkApp::init()
{
    BOOST_LOG_TRIVIAL(info) << "Initialized NetworkApp";
}

void NetworkApp::appLoopFunction()
{
    BOOST_LOG_TRIVIAL(info) << "Starting NetworkApp loop function.";

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

    BOOST_LOG_TRIVIAL(info) << "Ending NetworkApp loop function.";
}

}