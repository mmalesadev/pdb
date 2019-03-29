#include "AudiobookApp.h"
#include <boost/log/trivial.hpp>

namespace Pdb
{

AudiobookApp::AudiobookApp(AudioManager& audioManager, InputManager& inputManager, VoiceManager& voiceManager)
    : Pdb::App(audioManager, inputManager, voiceManager), audiobookPlayer_(audioManager, voiceManager)
{
    BOOST_LOG_TRIVIAL(info) << "Creating AudiobookApp.";
}

void AudiobookApp::init()
{
    synthesizeVoiceMessages();
    BOOST_LOG_TRIVIAL(info) << "Initialized AudiobookApp.";
}

void AudiobookApp::synthesizeVoiceMessages()
{
    /* Synthesizing lodaded audio track titles e.g. "harry potter" for "harry_potter.mp3" */
    for (auto& audioTrack : audiobookPlayer_.getAudioTracks())
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
            audiobookPlayer_.switchToPreviousAudiobook();
            BOOST_LOG_TRIVIAL(info) << "Audiobook switched to " << audiobookPlayer_.getCurrentTrack().getTrackName() << ".";
        }

        if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_E))
        {
            //audiobookPlayer_.playAudiobook(audioTracks_[1]);
            audiobookPlayer_.switchToNextAudiobook();
            BOOST_LOG_TRIVIAL(info) << "Audiobook switched to " << audiobookPlayer_.getCurrentTrack().getTrackName() << ".";
        }

        if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_W))
        {
            BOOST_LOG_TRIVIAL(info) << "Playing " << audiobookPlayer_.getCurrentTrack().getTrackName() << ".";
            audiobookPlayer_.playCurrentTrack();
        }      
    }
    BOOST_LOG_TRIVIAL(info) << "Ending AudiobookApp loop function.";
}

}