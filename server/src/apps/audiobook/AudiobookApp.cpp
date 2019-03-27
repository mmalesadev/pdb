#include "AudiobookApp.h"
#include <boost/log/trivial.hpp>

#include <boost/filesystem.hpp>

namespace filesystem = boost::filesystem;

namespace Pdb
{

AudiobookApp::AudiobookApp(AudioManager& audioManager, InputManager& inputManager, VoiceManager& voiceManager)
    : Pdb::App(audioManager, inputManager, voiceManager), audiobookPlayer_(audioManager, voiceManager)
{
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
}

void AudiobookApp::appLoopFunction()
{
    BOOST_LOG_TRIVIAL(info) << "Starting AudiobookApp loop function.";
    while(true)
    {
        inputManager_.update();
        if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_Q))
        {
            audioManager_.playAndGetAudioStream(voiceManager_.getSynthesizedVoiceAudioTracks().at("playing_audiobook"));
        }

        if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_W))
        {
            audioManager_.playAndGetAudioStream(audioTracks_[1]);
        }

        if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_E))
        {
            audiobookPlayer_.playAudiobook(audioTracks_[1]);
        }
        
    }
    BOOST_LOG_TRIVIAL(info) << "Ending AudiobookApp loop function.";
}

void AudiobookApp::loadTracks()
{
    filesystem::path path(std::string("../data/audiobooks/"));
    std::string fileExtension;

    if (!filesystem::exists(path))
    {
        BOOST_LOG_TRIVIAL(error) << "Directory with audiobooks not found.";
        return;
    }

    if (filesystem::is_directory(path))
    {
        filesystem::directory_iterator endIter;
        for (filesystem::directory_iterator dirItr(path); dirItr != endIter; ++dirItr)
        {
            if (filesystem::is_regular_file(dirItr->status()))
            {
                std::string trackName(dirItr->path().filename().c_str());
                fileExtension = (trackName.length() > 4) ? trackName.substr(trackName.length() - 3, 3) : "";

                if (fileExtension == "mp3" || fileExtension == "wav")
                {
                    AudioTrack audioTrack("../data/audiobooks/" + trackName);
                    audioTracks_.push_back(audioTrack);
                    BOOST_LOG_TRIVIAL(info) << trackName << " loaded.";
                }
            }
        }
    }
    BOOST_LOG_TRIVIAL(info) << audioTracks_.size() << " audio tracks successfully loaded.";
}

}