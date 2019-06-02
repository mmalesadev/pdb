#include "Config.h"
#include <boost/log/trivial.hpp>
#include <vector>

namespace Pdb
{

Config::Config()
{
	try
	{
		boost::property_tree::read_ini("../config.ini", pt_);
	}
	catch (std::exception &e)
	{
		BOOST_LOG_TRIVIAL(error) << e.what();
	}
	
	loggingLevel = pt_.get<std::string>("General.loggingLevel");
	inputMode = pt_.get<std::string>("General.inputMode");
	volumeForAwsSynthesized = pt_.get<float>("SynthesizedAudio.volume");
	volumeForAudiobooks = pt_.get<float>("AudiobookAudio.volume");
	masterVolume = pt_.get<float>("MasterVolume.masterVolume");
}


}