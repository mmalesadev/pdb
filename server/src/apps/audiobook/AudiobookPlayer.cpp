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
    currentState_ = State::CHOOSING;

    // CHOOSING STATE
    std::vector< std::pair<InputManager::Button, std::function<void()> > > choosingStateActions;
    choosingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_Q, std::bind(&AudiobookPlayer::switchToPreviousAudiobook, this)));
    choosingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_W, std::bind(&AudiobookPlayer::playCurrentTrack, this)));
    choosingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_E, std::bind(&AudiobookPlayer::switchToNextAudiobook, this)));
    availableActions_.insert(std::make_pair(State::CHOOSING, std::move(choosingStateActions)));

    // PLAYING STATE
    std::vector< std::pair<InputManager::Button, std::function<void()> > > playingStateActions;
    playingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_Q, std::bind(&AudiobookPlayer::rewinding, this)));
    playingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_E, std::bind(&AudiobookPlayer::fastForwarding, this)));
    playingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_S, std::bind(&AudiobookPlayer::togglePause, this)));
    availableActions_.insert(std::make_pair(State::PLAYING, std::move(playingStateActions)));

    // REWINDING STATE
    std::vector< std::pair<InputManager::Button, std::function<void()> > > rewindingStateActions;
    rewindingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_Q, std::bind(&AudiobookPlayer::increaseRewindingSpeed, this)));
    rewindingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_W, std::bind(&AudiobookPlayer::playCurrentTrack, this)));
    rewindingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_E, std::bind(&AudiobookPlayer::decreaseRewindingSpeed, this)));
    rewindingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_S, std::bind(&AudiobookPlayer::togglePause, this)));
    availableActions_.insert(std::make_pair(State::REWINDING, std::move(rewindingStateActions)));

    // FAST_FORWARDING STATE
    std::vector< std::pair<InputManager::Button, std::function<void()> > > fastForwardingStateActions;
    fastForwardingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_Q, std::bind(&AudiobookPlayer::decreaseRewindingSpeed, this)));
    fastForwardingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_W, std::bind(&AudiobookPlayer::playCurrentTrack, this)));
    fastForwardingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_E, std::bind(&AudiobookPlayer::increaseRewindingSpeed, this)));
    fastForwardingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_S, std::bind(&AudiobookPlayer::togglePause, this)));
    availableActions_.insert(std::make_pair(State::FAST_FORWARDING, std::move(fastForwardingStateActions)));

    // PAUSED STATE
    std::vector< std::pair<InputManager::Button, std::function<void()> > > pausedStateActions;
    pausedStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_S, std::bind(&AudiobookPlayer::togglePause, this)));
    availableActions_.insert(std::make_pair(State::PAUSED, std::move(pausedStateActions)));
}

void AudiobookPlayer::playAudiobook(AudioTrack& audioTrack)
{
    BOOST_LOG_TRIVIAL(info) << "Playing audiotrack: " << audioTrack.getTrackName() << " (" << audioTrack.getFilePath() << ")";
    currentlyPlayedAudioStream_ = audioManager_.playMultipleAndGetLastAudioStream(std::vector<AudioTrack>({
            voiceManager_.getSynthesizedVoiceAudioTracks().at("playing_audiobook"),
            voiceManager_.getSynthesizedVoiceAudioTracks().at(audioTrack.getTrackName()),
            audioTrack
        })).get();
    auto audiobookFinishCallback = [this]()
    {
        currentlyPlayedAudioStream_->getAudioStreamFuture().get();
        changeStateTo(State::CHOOSING);
        BOOST_LOG_TRIVIAL(info) << "Finished playing audiotrack.";
    };
    std::thread(audiobookFinishCallback).detach();
}

void AudiobookPlayer::togglePause()
{
    BOOST_LOG_TRIVIAL(info) << "Toggling audiotrack pause: " << audioTracks_[currentTrackIndex_].getTrackName();
    changeStateTo(State::PAUSED);
    if (currentlyPlayedAudioStream_->isPaused()) audioManager_.playSync(voiceManager_.getSynthesizedVoiceAudioTracks().at("unpausing_audiobook"));
    else audioManager_.playAndGetAudioStream(voiceManager_.getSynthesizedVoiceAudioTracks().at("pausing_audiobook"));
    currentlyPlayedAudioStream_->pauseToggle();
    BOOST_LOG_TRIVIAL(info) << "Paused = " << currentlyPlayedAudioStream_->isPaused();
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

    audioManager_.playMultipleAndGetLastAudioStream(std::vector<AudioTrack>({ 
        voiceManager_.getSynthesizedVoiceAudioTracks().at("chosen_next"),
        voiceManager_.getSynthesizedVoiceAudioTracks().at(getCurrentTrack().getTrackName())
    }));
}

void AudiobookPlayer::switchToPreviousAudiobook()
{
    if (currentTrackIndex_ == 0)
        currentTrackIndex_ = audioTracks_.size() - 1;  
    else
        --currentTrackIndex_;

    audioManager_.playMultipleAndGetLastAudioStream(std::vector<AudioTrack>({ 
        voiceManager_.getSynthesizedVoiceAudioTracks().at("chosen_previous"),
        voiceManager_.getSynthesizedVoiceAudioTracks().at(getCurrentTrack().getTrackName())
    }));
}

void AudiobookPlayer::playCurrentTrack()
{
    changeStateTo(State::PLAYING);
    playAudiobook(audioTracks_[currentTrackIndex_]);
}

std::vector< std::pair<InputManager::Button, std::function<void()> > > & AudiobookPlayer::getAvailableActions()
{
    return availableActions_.at(currentState_);
}

void AudiobookPlayer::increaseRewindingSpeed()
{
}
void AudiobookPlayer::decreaseRewindingSpeed()
{
}
void AudiobookPlayer::rewinding()
{
    changeStateTo(State::REWINDING);
    BOOST_LOG_TRIVIAL(info) << "Rewinding.";
}
void AudiobookPlayer::fastForwarding()
{
    changeStateTo(State::FAST_FORWARDING);
    BOOST_LOG_TRIVIAL(info) << "Fast forwarding.";
}
void AudiobookPlayer::stopAudiobook()
{
    changeStateTo(State::CHOOSING);
}

void AudiobookPlayer::printState()
{
    BOOST_LOG_TRIVIAL(info) << static_cast<std::underlying_type<State>::type>(currentState_);
}

}