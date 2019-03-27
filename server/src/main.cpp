#include "Server.h"
#include "Config.h"

#include "apps/network/NetworkApp.h"
#include "apps/audiobook/AudiobookApp.h"

#include "boost/log/trivial.hpp"
#include "boost/log/utility/setup.hpp"

#include <memory>
#include <iomanip>

int main(int argc, char* argv[])
{
    /* Initializing logger */
    boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
    boost::log::add_console_log(
        std::cout,
        boost::log::keywords::format = "[%TimeStamp%][%Severity%] %Message%",
        boost::log::keywords::auto_flush = true
    );
    std::time_t time = std::time(nullptr);
    std::stringstream logFileName;
    logFileName << "pdbserver.log" << std::put_time(std::localtime(&time), "%Y-%m-%dT%H.%M.%S");
    boost::log::add_file_log(
        boost::log::keywords::file_name = "logs/" + logFileName.str(),
        boost::log::keywords::rotation_size = 1 * 1024 * 1024,
        boost::log::keywords::max_size = 20 * 1024 * 1024,
        boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
        boost::log::keywords::format = "[%TimeStamp%][%Severity%] %Message%",
        boost::log::keywords::auto_flush = true
    );
    boost::log::add_common_attributes();
    boost::log::core::get()->set_filter(
        boost::log::trivial::severity >= boost::log::trivial::info
    );

    /* Starting app */
    Pdb::Config::getInstance();
    Pdb::Server server;

    server.registerApp("network", std::make_unique<Pdb::NetworkApp>(server.getAudioManager(), server.getInputManager(), server.getVoiceManager()));
    server.registerApp("audiobook", std::make_unique<Pdb::AudiobookApp>(server.getAudioManager(), server.getInputManager(), server.getVoiceManager()));

    server.run();
    
    return 0;
}

