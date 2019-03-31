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

    changeStateTo(CHOOSING);
    audioManager_.playMultipleAndGetLastAudioStream(std::vector<AudioTrack>({ 
        voiceManager_.getSynthesizedVoiceAudioTracks().at("choosing_audiobooks"),
        voiceManager_.getSynthesizedVoiceAudioTracks().at(audiobookPlayer_.getCurrentTrack().getTrackName())
     }));

    while(true)
    {
        inputManager_.update();

        if (inputManager_.isButtonPressed(InputManager::Button::REWIND_BUTTON))
        {
            if (currentState_ == CHOOSING)
            {
                audiobookPlayer_.switchToPreviousAudiobook();
                BOOST_LOG_TRIVIAL(info) << "Audiobook switched to " << audiobookPlayer_.getCurrentTrack().getTrackName() << ".";         
            }
            else if (currentState_ == PLAYING)
            {
                // CHANGE STATE TO REWIND
            }
            else if (currentState_ == REWINDING)
            {
                // INCREASE REWIND SPEED
            }
            else if (currentState_ == REWINDING_FORWARD)
            {
                // DECREASE REWIND SPEED
            }
            else if (currentState_ == PAUSE)
            {
                // CHANGE STATE TO REWIND
            }
        }

        if (inputManager_.isButtonPressed(InputManager::Button::PLAY_BUTTON))
        {
            if (currentState_ == PLAYING)
            {
                // UNDEFINED
            }
            else
            {
                BOOST_LOG_TRIVIAL(info) << "Playing " << audiobookPlayer_.getCurrentTrack().getTrackName() << ".";
                audiobookPlayer_.playCurrentTrack();
                changeStateTo(PLAYING);
            }
        }

        if (inputManager_.isButtonPressed(InputManager::Button::PAUSE_BUTTON))
        {
            if (currentState_ == CHOOSING)
            {
                // UNDEFINED
            }
            else if (currentState_ == PAUSE)
            {
                changeStateTo(CHOOSING);
            }
            else
            {
                BOOST_LOG_TRIVIAL(info) << "Pausing " << audiobookPlayer_.getCurrentTrack().getTrackName() << ".";
                audiobookPlayer_.pauseCurrentTrack();
                changeStateTo(PAUSE);               
            }
        }

        if (inputManager_.isButtonPressed(InputManager::Button::REWIND_FORWARD_BUTTON))
        {
            if (currentState_ == CHOOSING)
            {
                audiobookPlayer_.switchToNextAudiobook();
                BOOST_LOG_TRIVIAL(info) << "Audiobook switched to " << audiobookPlayer_.getCurrentTrack().getTrackName() << ".";
            }
            else if (currentState_ == PLAYING)
            {
                // CHANGE STATE TO REWIND_FORWARD
            }
            else if (currentState_ == REWINDING)
            {
                // DECREASE REWINDING SPEED
            }
            else if (currentState_ == REWINDING_FORWARD)
            {
                // INCREASE REWINDING SPEED
            }
            else if (currentState_ == PAUSE)
            {
                // CHANGE STATE TO REWIND_FORWARD
            }
        }
    }

    BOOST_LOG_TRIVIAL(info) << "Ending AudiobookApp loop function.";
}

void AudiobookApp::changeStateTo(States state)
{
    if (state == CHOOSING) currentState_ = CHOOSING;
    else if (state == PLAYING) currentState_ = PLAYING;
    else if (state == REWINDING) currentState_ = REWINDING;
    else if (state == REWINDING_FORWARD) currentState_ = REWINDING_FORWARD;
    else if (state == PAUSE) currentState_ = PAUSE;
}

}