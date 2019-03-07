#include "PDBAudiobook.h"
#include <boost/log/trivial.hpp>

PDBAudiobook::PDBAudiobook(AudioManager& audioManager, InputManager& inputManager) 
    : PDBApp(inputManager), audiobookPlayer_(audioManager)
{
    BOOST_LOG_TRIVIAL(info) << "Creating PDBAudiobook app.";
}

void PDBAudiobook::init()
{
    loadTracks();
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
            AudioTrack audiotrack("stereo_sound.wav");
            audiobookPlayer_.playAudiobook(audiotrack);
        }
    }
    BOOST_LOG_TRIVIAL(info) << "Ending PDBAudiobook loop function.";
}

void PDBAudiobook::loadTracks()
{
    DIR* d;
    dirent* dir;
    std::string fileExtension = "";
    d = opendir("/home/piotr/Documents/Projects/pdb/server/data/");
    while ((dir = readdir(d)) != NULL)
    {
        std::string trackName(dir->d_name);
        fileExtension = (trackName.length() > 4) ? trackName.substr(trackName.length() - 3, 3) : "";
        if (fileExtension == "mp3" || fileExtension == "wav")
        {
            AudioTrack audioTrack(trackName);
            audioTracks_.push_back(audioTrack);
            BOOST_LOG_TRIVIAL(info) << trackName << " loaded.";
        }
    }
    closedir(d);
}
