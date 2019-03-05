#include "PDBAudiobook.h"
#include <boost/log/trivial.hpp>

PDBAudiobook::PDBAudiobook(AudioManager& audioManager, InputManager& inputManager) 
    : PDBApp(inputManager), audiobookPlayer_(audioManager)
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

    while(true)
    {
        inputManager_.update();
        if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_Q))
        {
            AudioTrack audiotrack("klapsczang.wav");
            audiobookPlayer_.playAudiobook(audiotrack);
        }
    }
    BOOST_LOG_TRIVIAL(info) << "Ending PDBAudiobook loop function.";
}

void playMessage(const std::string & message)
{
    std::string milenaMessageCall = "milena_say " + message;
    system(milenaMessageCall.c_str());
}