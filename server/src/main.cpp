#include "Server.h"
#include "Config.h"

#include "apps/network/NetworkApp.h"
#include "apps/audiobook/AudiobookApp.h"
#include "apps/clock/ClockApp.h"

#include "boost/log/trivial.hpp"
#include "boost/log/utility/setup.hpp"

#include <memory>
#include <iomanip>
#include <string>

int main(int argc, char* argv[])
{
    /* Initializing config file variables */
    Pdb::Config::getInstance();

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
    boost::log::trivial::severity_level severityLevel;
    std::string configLoggingLevel = Pdb::Config::getInstance().loggingLevel;

    if (configLoggingLevel == "info") severityLevel = boost::log::trivial::info;
    else if (configLoggingLevel == "debug") severityLevel = boost::log::trivial::debug;
    else if (configLoggingLevel == "trace") severityLevel = boost::log::trivial::trace;
    else
    {
        BOOST_LOG_TRIVIAL(error) << "Config: " << configLoggingLevel << " is a wrong loggingLevel value.";
		exit(0);
    }

    boost::log::core::get()->set_filter(
        boost::log::trivial::severity >= severityLevel
    );

    /* Starting app */
    Pdb::Server server;

    server.registerApp("network", std::make_unique<Pdb::NetworkApp>(server.getVoiceManager()));
    server.registerApp("audiobook", std::make_unique<Pdb::AudiobookApp>(server.getVoiceManager()));
    server.registerApp("clock", std::make_unique<Pdb::ClockApp>(server.getVoiceManager()));

    server.run();
    
    return 0;
}

