#include "Config.h"
#include <boost/log/trivial.hpp>

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
}


}