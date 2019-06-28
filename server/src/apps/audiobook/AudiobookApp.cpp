#include "AudiobookApp.h"
#include <boost/log/trivial.hpp>

namespace Pdb
{

AudiobookApp::AudiobookApp(VoiceManager& voiceManager)
    : Pdb::App(voiceManager), audiobookPlayer_(audioManager_, voiceManager)
{
    BOOST_LOG_TRIVIAL(info) << "Creating AudiobookApp.";
}

void AudiobookApp::init()
{
    synthesizeVoiceMessages();
    AudioTask* initialAudioTask = audioManager_.play({
        voiceManager_.getSynthesizedVoiceAudioTracks().at("choosing_audiobooks"),
        voiceManager_.getSynthesizedVoiceAudioTracks().at(audiobookPlayer_.getCurrentTrack().getTrackName())
    });
    audiobookPlayer_.setCurrentAudioTask(initialAudioTask);
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
    voiceManager_.synthesizeVoiceMessage("<speak>Zakończono odtwarzanie: </speak>", "../data/synthesized_sounds/apps/audiobook/messages/pl", "stopping_audiobook");
    voiceManager_.synthesizeVoiceMessage("<speak>Przewijanie do przodu. </speak>", "../data/synthesized_sounds/apps/audiobook/messages/pl", "fast_forwarding");
    voiceManager_.synthesizeVoiceMessage("<speak>Przewijanie do tyłu. </speak>", "../data/synthesized_sounds/apps/audiobook/messages/pl", "rewinding");
    voiceManager_.synthesizeVoiceMessage("<speak>2-krotne </speak>", "../data/synthesized_sounds/apps/audiobook/messages/pl", "2x");
    voiceManager_.synthesizeVoiceMessage("<speak>4-krotne </speak>", "../data/synthesized_sounds/apps/audiobook/messages/pl", "4x");
    voiceManager_.synthesizeVoiceMessage("<speak>8-krotne </speak>", "../data/synthesized_sounds/apps/audiobook/messages/pl", "8x");
    voiceManager_.synthesizeVoiceMessage("<speak>16-krotne </speak>", "../data/synthesized_sounds/apps/audiobook/messages/pl", "16x");
    voiceManager_.synthesizeVoiceMessage("<speak>32-krotne </speak>", "../data/synthesized_sounds/apps/audiobook/messages/pl", "32x");
    voiceManager_.synthesizeVoiceMessage("<speak>64-krotne </speak>", "../data/synthesized_sounds/apps/audiobook/messages/pl", "64x");
    voiceManager_.synthesizeVoiceMessage("<speak>128-krotne </speak>", "../data/synthesized_sounds/apps/audiobook/messages/pl", "128x");
}

void AudiobookApp::appLoopFunction()
{
    BOOST_LOG_TRIVIAL(info) << "Starting AudiobookApp loop function.";
        
    while(true)
    {
        inputManager_.update();
        auto& availableActions = audiobookPlayer_.getAvailableActions();
        for (auto& action : availableActions) 
        {
            if (inputManager_.isButtonPressed(action.first))
                action.second();
        }

        if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_X))
        {
            audiobookPlayer_.printState();
            audioManager_.printAllStreamsInfo();
        }
    }

    BOOST_LOG_TRIVIAL(info) << "Ending AudiobookApp loop function.";
}

}