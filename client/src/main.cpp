#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //--------------------------------------------------------------------------------------
    try
    {
        if(argc != 2)
        {
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }

        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints =
            resolver.resolve(argv[1], "7171");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        for(;;)
        {
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if(error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer
            else if(error)
                throw boost::system::system_error(error); // Some other error.
            std::cout.write(buf.data(), len);
            printf("loler\n");
        }
    }
    catch(std::exception& e)
    {
            std::cerr << e.what() << std::endl;
    }

    //------------------------------------------------------------------------
    w.show();

    return a.exec();
}

