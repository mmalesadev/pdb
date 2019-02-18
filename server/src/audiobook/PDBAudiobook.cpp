#include "PDBAudiobook.h"
#include <boost/log/trivial.hpp>

PDBAudiobook::PDBAudiobook(AudioManager& audioManager) : audiobookPlayer_(audioManager)
{
    BOOST_LOG_TRIVIAL(info) << "Creating PDBAudiobook app.";
}

void PDBAudiobook::init()
{
    BOOST_LOG_TRIVIAL(info) << "Initialized PDBAudiobook.";
}

void PDBAudiobook::appLoopFunction()
{
    BOOST_LOG_TRIVIAL(info) << "Starting PDBAudiobook loop function.";
    AudioTrack audioTrack("klapsczang.wav");
    audiobookPlayer_.playAudiobook(audioTrack);

    while(true)
    {

    }
    BOOST_LOG_TRIVIAL(info) << "Ending PDBAudiobook loop function.";
}

void playMessage(const std::string & message)
{
    std::string milenaMessageCall = "milena_say " + message;
    system(milenaMessageCall.c_str());
}