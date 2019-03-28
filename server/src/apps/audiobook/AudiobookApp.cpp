#include "AudiobookApp.h"
#include <boost/log/trivial.hpp>

#include <boost/filesystem.hpp>

namespace filesystem = boost::filesystem;

namespace Pdb
{

AudiobookApp::AudiobookApp(AudioManager& audioManager, InputManager& inputManager, VoiceManager& voiceManager)
    : Pdb::App(audioManager, inputManager, voiceManager), audiobookPlayer_(audioManager, voiceManager)
{
    this->currentIndex_ = 0;
    BOOST_LOG_TRIVIAL(info) << "Creating AudiobookApp.";
}

void AudiobookApp::init()
{
    loadTracks();
    synthesizeVoiceMessages();
    BOOST_LOG_TRIVIAL(info) << "Initialized AudiobookApp.";
}

void AudiobookApp::synthesizeVoiceMessages()
{
    /* Synthesizing lodaded audio track titles e.g. "harry potter" for "harry_potter.mp3" */
    for (auto& audioTrack : audioTracks_)
    {
        std::string message = audioTrack.getTrackName();
        std::replace(message.begin(), message.end(), '_', ' ');
        voiceManager_.synthesizeVoiceMessage("<speak>" + message + "</speak>", "../data/synthesized_sounds/apps/audiobook/audiobook_titles", audioTrack.getTrackName());
    }

    /* Synthesizing helper messages for blind people e.g. "playing audiobook" */
    voiceManager_.synthesizeVoiceMessage("<speak>Odtwarzanie książki<break time=\"50ms\"/></speak>", "../data/synthesized_sounds/apps/audiobook/messages/pl", "playing_audiobook");
    voiceManager_.synthesizeVoiceMessage("<speak>Wybrano książkę<break time=\"50ms\"/></speak>", "../data/synthesized_sounds/apps/audiobook/messages/pl", "chosen_audiobook");
}

void AudiobookApp::appLoopFunction()
{
    BOOST_LOG_TRIVIAL(info) << "Starting AudiobookApp loop function.";

    while(true)
    {
        inputManager_.update();
        if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_Q))
        {
            //audioManager_.playAndGetAudioStream(voiceManager_.getSynthesizedVoiceAudioTracks().at("playing_audiobook"));
            this->switchAudiobook('L');
            BOOST_LOG_TRIVIAL(info) << "Audiobook switched to " << this->audioTracks_[currentIndex_].getTrackName() << ".";
        }

        if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_E))
        {
            //audiobookPlayer_.playAudiobook(audioTracks_[1]);
            this->switchAudiobook('R');
            BOOST_LOG_TRIVIAL(info) << "Audiobook switched to " << this->audioTracks_[currentIndex_].getTrackName() << ".";
        }

        if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_W))
        {
            BOOST_LOG_TRIVIAL(info) << "Playing " << this->audioTracks_[currentIndex_].getTrackName() << ".";
            this->playCurrentTrack();
        }

        if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_S))
        {
            
        }
        
    }
    BOOST_LOG_TRIVIAL(info) << "Ending AudiobookApp loop function.";
}

}