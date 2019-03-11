#include "AudiobookApp.h"
#include <boost/log/trivial.hpp>

namespace Pdb
{

AudiobookApp::AudiobookApp(AudioManager& audioManager, InputManager& inputManager, VoiceManager& voiceManager)
    : Pdb::App(audioManager, inputManager, voiceManager), audiobookPlayer_(audioManager)
{
    BOOST_LOG_TRIVIAL(info) << "Creating AudiobookApp.";
}

void AudiobookApp::init()
{
    BOOST_LOG_TRIVIAL(info) << "Initialized AudiobookApp.";
    voiceManager_.synthesizeVoiceMessage("Test message", "../data/synthesized_sounds/audiobook_titles", "test_message");
}

void AudiobookApp::appLoopFunction()
{
    BOOST_LOG_TRIVIAL(info) << "Starting AudiobookApp loop function.";
    while(true)
    {
        inputManager_.update();
        if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_Q))
        {
            AudioTrack audiotrack("klapsczang.wav");
            audiobookPlayer_.playAudiobook(audiotrack);
        }
    }
    BOOST_LOG_TRIVIAL(info) << "Ending AudiobookApp loop function.";
}

}