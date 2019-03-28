#include "AudiobookPlayer.h"
#include <boost/log/trivial.hpp>
#include <future>
#include <memory>
#include <boost/filesystem.hpp>

namespace filesystem = boost::filesystem;

namespace Pdb
{

AudiobookPlayer::AudiobookPlayer(AudioManager& audioManager, VoiceManager& voiceManager) 
    : audioManager_(audioManager), voiceManager_(voiceManager)
{
    this->loadTracks();
    currentTrackIndex_ = 0;
}

void AudiobookPlayer::playAudiobook(AudioTrack& audioTrack)
{
    BOOST_LOG_TRIVIAL(info) << "Playing audiotrack: " << audioTrack.getTrackName() << " (" << audioTrack.getFilePath() << ")";
    currentlyPlayedAudioStream_ = audioManager_.playMultipleAndGetLastAudioStream(std::vector<AudioTrack>({
            voiceManager_.getSynthesizedVoiceAudioTracks().at("playing_audiobook"),
            voiceManager_.getSynthesizedVoiceAudioTracks().at(audioTrack.getTrackName()),
            audioTrack
        })).get();
}

void AudiobookPlayer::pauseCurrentTrack()
{
    BOOST_LOG_TRIVIAL(info) << "Pausing audiotrack: " << audioTracks_[currentTrackIndex_].getTrackName();
    currentlyPlayedAudioStream_->pauseToggle();
}

void AudiobookPlayer::loadTracks()
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
                    AudioTrack audioTrack("../data/audiobooks/" + trackName, Config::getInstance().masterVolumeForAudiobooks);
                    audioTracks_.push_back(audioTrack);
                    BOOST_LOG_TRIVIAL(info) << trackName << " loaded.";
                }
            }
        }
    }
    BOOST_LOG_TRIVIAL(info) << audioTracks_.size() << " audio tracks successfully loaded.";
}

void AudiobookPlayer::switchToNextAudiobook()
{
    if (currentTrackIndex_ == audioTracks_.size() - 1)
        currentTrackIndex_ = 0;
    else
        ++currentTrackIndex_;
}

void AudiobookPlayer::switchToPreviousAudiobook()
{
    if (currentTrackIndex_ == 0)
        currentTrackIndex_ = audioTracks_.size() - 1;  
    else
        --currentTrackIndex_;
}

void AudiobookPlayer::playCurrentTrack()
{
    playAudiobook(audioTracks_[currentTrackIndex_]);
}

}