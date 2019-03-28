#include "AudiobookPlayer.h"
#include <boost/log/trivial.hpp>
#include <future>
#include <memory>

namespace Pdb
{

AudiobookPlayer::AudiobookPlayer(AudioManager& audioManager, VoiceManager& voiceManager) 
    : audioManager_(audioManager), voiceManager_(voiceManager)
{
    this->loadTracks();
}

void AudiobookPlayer::playAudiobook(AudioTrack& audioTrack)
{
    currentlyPlayedAudioStream_ = audioManager_.playMultipleAndGetLastAudioStream(std::vector<AudioTrack>({
            voiceManager_.getSynthesizedVoiceAudioTracks().at("playing_audiobook"),
            voiceManager_.getSynthesizedVoiceAudioTracks().at(audioTrack.getTrackName()),
            audioTrack
        }));
}

void AudiobookPlayer::pauseAudiobook()
{
    
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
                    AudioTrack audioTrack("../data/audiobooks/" + trackName);
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
    if (currentIndex_ == audioTracks_.size() - 1)
    {
        currentIndex_ = 0;
    }
    
    else
    {
        ++currentIndex_;
    }
}

void AudiobookPlayer::switchToPreviousAudiobook()
{
    if (currentIndex_ == 0)
    {
        currentIndex_ = audioTracks_.size() - 1;
    }
    
    else
    {
        --currentIndex_;
    }
}

void AudiobookPlayer::playCurrentTrack()
{
    audioManager_.playAndGetAudioStream(this->audioTracks_[currentIndex_]);
}

}