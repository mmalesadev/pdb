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
    voiceManager_.synthesizeVoiceMessage("<speak>Odtwarzanie książki: </speak>", "../data/synthesized_sounds/apps/audiobook/messages/pl", "playing_audiobook");
    voiceManager_.synthesizeVoiceMessage("<speak>Wybrana książka: </speak>", "../data/synthesized_sounds/apps/audiobook/messages/pl", "chosen_audiobook");
    voiceManager_.synthesizeVoiceMessage("<speak>Wybieranie książek. </speak>", "../data/synthesized_sounds/apps/audiobook/messages/pl", "choosing_audiobooks");
    voiceManager_.synthesizeVoiceMessage("<speak>Wybrano kolejną: </speak>", "../data/synthesized_sounds/apps/audiobook/messages/pl", "chosen_next");
    voiceManager_.synthesizeVoiceMessage("<speak>Wybrano poprzednią: </speak>", "../data/synthesized_sounds/apps/audiobook/messages/pl", "chosen_previous");
    voiceManager_.synthesizeVoiceMessage("<speak>Zatrzymano odtwarzanie. </speak>", "../data/synthesized_sounds/apps/audiobook/messages/pl", "pausing_audiobook");
    voiceManager_.synthesizeVoiceMessage("<speak>Wznawianie odtwarzania: </speak>", "../data/synthesized_sounds/apps/audiobook/messages/pl", "unpausing_audiobook");
}

void AudiobookApp::appLoopFunction()
{
    BOOST_LOG_TRIVIAL(info) << "Starting AudiobookApp loop function.";

    audioManager_.playMultipleAndGetLastAudioStream(std::vector<AudioTrack>({ 
        voiceManager_.getSynthesizedVoiceAudioTracks().at("choosing_audiobooks"),
        voiceManager_.getSynthesizedVoiceAudioTracks().at(audiobookPlayer_.getCurrentTrack().getTrackName())
     }));

    while(true)
    {
        inputManager_.update();
        if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_Q))
        {
            audiobookPlayer_.switchToPreviousAudiobook();
            BOOST_LOG_TRIVIAL(info) << "Audiobook switched to " << audiobookPlayer_.getCurrentTrack().getTrackName() << ".";
        }

        if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_W))
        {
            BOOST_LOG_TRIVIAL(info) << "Playing " << audiobookPlayer_.getCurrentTrack().getTrackName() << ".";
            audiobookPlayer_.playCurrentTrack();
        }

        if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_S))
        {
            BOOST_LOG_TRIVIAL(info) << "Pausing " << audiobookPlayer_.getCurrentTrack().getTrackName() << ".";
            audiobookPlayer_.pauseCurrentTrack();
        }

        if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_E))
        {
            audiobookPlayer_.switchToNextAudiobook();
            BOOST_LOG_TRIVIAL(info) << "Audiobook switched to " << audiobookPlayer_.getCurrentTrack().getTrackName() << ".";
        }
    }

    BOOST_LOG_TRIVIAL(info) << "Ending AudiobookApp loop function.";
}

}