#include "AudiobookApp.h"
#include <boost/log/trivial.hpp>

namespace Pdb
{

AudiobookApp::AudiobookApp(AudioManager& audioManager) : audiobookPlayer_(audioManager)
{
    BOOST_LOG_TRIVIAL(info) << "Creating AudiobookApp app.";
}

void AudiobookApp::init()
{
    BOOST_LOG_TRIVIAL(info) << "Initialized AudiobookApp.";
}

void AudiobookApp::appLoopFunction()
{
    BOOST_LOG_TRIVIAL(info) << "Starting AudiobookApp loop function.";
    while(true)
    {

    }
    BOOST_LOG_TRIVIAL(info) << "Ending AudiobookApp loop function.";
}

}