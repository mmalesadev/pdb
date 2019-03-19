#include "AudiobookApp.h"
#include <boost/log/trivial.hpp>
#include <boost/filesystem.hpp>

namespace Pdb
{

AudiobookApp::AudiobookApp(AudioManager& audioManager, InputManager& inputManager, VoiceManager& voiceManager)
    : Pdb::App(audioManager, inputManager, voiceManager), audiobookPlayer_(audioManager)
{
    BOOST_LOG_TRIVIAL(info) << "Creating AudiobookApp.";
}

void AudiobookApp::init()
{
    loadTracks();
    synthesizeTrackTitles();
    BOOST_LOG_TRIVIAL(info) << "Initialized AudiobookApp.";
}

void AudiobookApp::synthesizeTrackTitles()
{
    for (auto& audioTrack : audioTracks_)
    {
        std::string trackName = audioTrack.getTrackName();
        std::string message = trackName;
        std::replace(message.begin(), message.end(), '_', ' ');
        std::string synthesizedAudiobookTitlesDir = "../data/synthesized_sounds/audiobook_titles";
        boost::filesystem::path synthesizedAudiobookTitleFile(synthesizedAudiobookTitlesDir + "/" + audioTrack.getTrackName() + ".mp3");
        if (!boost::filesystem::exists(synthesizedAudiobookTitleFile))
        {
            BOOST_LOG_TRIVIAL(info) << "Synthesizing audiobook title for " << trackName << ".\n";
            voiceManager_.synthesizeVoiceMessage(message, "../data/synthesized_sounds/audiobook_titles", trackName);
        }
        else
        {
            BOOST_LOG_TRIVIAL(info) << "Audiobook title already synthesized (" << synthesizedAudiobookTitleFile << ").";
        }
        
    }
}

void AudiobookApp::appLoopFunction()
{
    BOOST_LOG_TRIVIAL(info) << "Starting AudiobookApp loop function.";
    while(true)
    {
        inputManager_.update();
        if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_Q))
        {
            AudioTrack audiotrack("../data/klapsczang.wav");
            audiobookPlayer_.playAudiobook(audiotrack);
        }
    }
    BOOST_LOG_TRIVIAL(info) << "Ending AudiobookApp loop function.";
}

void AudiobookApp::loadTracks()
{
    BOOST_LOG_TRIVIAL(info) << "Loading audio tracks.";
    DIR* d;
    dirent* dir;
    std::string fileExtension = "";
    std::string path = "../data/audiobooks/";
    d = opendir(path.c_str());
    while ((dir = readdir(d)) != NULL)
    {
        std::string trackName(dir->d_name);
        fileExtension = (trackName.length() > 4) ? trackName.substr(trackName.length() - 3, 3) : "";
        if (fileExtension == "mp3" || fileExtension == "wav")
        {
            AudioTrack audioTrack(path + trackName);
            audioTracks_.push_back(audioTrack);
            BOOST_LOG_TRIVIAL(info) << audioTrack.getFilePath() << " loaded.";
        }
    }
    closedir(d);
}

}