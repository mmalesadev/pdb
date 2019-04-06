#include "AudiobookPlayer.h"
#include <boost/log/trivial.hpp>
#include <future>
#include <memory>
#include <boost/filesystem.hpp>

namespace filesystem = boost::filesystem;

namespace Pdb
{

AudiobookPlayer::AudiobookPlayer(AudioManager& audioManager, VoiceManager& voiceManager) 
    : audioManager_(audioManager), voiceManager_(voiceManager), currentlyPlayedAudioStream_(nullptr), fastForwardingSpeed_(0)
{
    this->loadTracks();
    currentTrackIndex_ = 0;
    currentState_ = State::CHOOSING;

    // CHOOSING STATE
    std::vector< std::pair<InputManager::Button, std::function<void()> > > choosingStateActions;
    choosingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_Q, std::bind(&AudiobookPlayer::switchToPreviousAudiobook, this)));
    choosingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_W, std::bind(&AudiobookPlayer::playCurrentTrack, this)));
    choosingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_E, std::bind(&AudiobookPlayer::switchToNextAudiobook, this)));
    choosingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_UP, std::bind(&AudioManager::increaseMasterVolume, &audioManager_)));
    choosingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_DOWN, std::bind(&AudioManager::decreaseMasterVolume, &audioManager_)));
    availableActions_.insert(std::make_pair(State::CHOOSING, std::move(choosingStateActions)));

    // PLAYING STATE
    std::vector< std::pair<InputManager::Button, std::function<void()> > > playingStateActions;
    playingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_Q, std::bind(&AudiobookPlayer::rewind, this)));
    playingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_E, std::bind(&AudiobookPlayer::fastForward, this)));
    playingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_S, std::bind(&AudiobookPlayer::togglePause, this)));
    playingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_UP, std::bind(&AudioManager::increaseMasterVolume, &audioManager_)));
    playingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_DOWN, std::bind(&AudioManager::decreaseMasterVolume, &audioManager_)));
    availableActions_.insert(std::make_pair(State::PLAYING, std::move(playingStateActions)));

    // REWINDING STATE
    std::vector< std::pair<InputManager::Button, std::function<void()> > > rewindingStateActions;
    rewindingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_Q, std::bind(&AudiobookPlayer::rewind, this)));
    rewindingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_W, std::bind(&AudiobookPlayer::playCurrentTrack, this)));
    rewindingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_E, std::bind(&AudiobookPlayer::fastForward, this)));
    rewindingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_S, std::bind(&AudiobookPlayer::togglePause, this)));
    rewindingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_UP, std::bind(&AudioManager::increaseMasterVolume, &audioManager_)));
    rewindingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_DOWN, std::bind(&AudioManager::decreaseMasterVolume, &audioManager_)));
    availableActions_.insert(std::make_pair(State::REWINDING, std::move(rewindingStateActions)));

    // FAST_FORWARDING STATE
    std::vector< std::pair<InputManager::Button, std::function<void()> > > fastForwardingStateActions;
    fastForwardingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_Q, std::bind(&AudiobookPlayer::rewind, this)));
    fastForwardingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_W, std::bind(&AudiobookPlayer::playCurrentTrack, this)));
    fastForwardingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_E, std::bind(&AudiobookPlayer::fastForward, this)));
    fastForwardingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_S, std::bind(&AudiobookPlayer::togglePause, this)));
    fastForwardingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_UP, std::bind(&AudioManager::increaseMasterVolume, &audioManager_)));
    fastForwardingStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_DOWN, std::bind(&AudioManager::decreaseMasterVolume, &audioManager_)));
    availableActions_.insert(std::make_pair(State::FAST_FORWARDING, std::move(fastForwardingStateActions)));

    // PAUSED STATE
    std::vector< std::pair<InputManager::Button, std::function<void()> > > pausedStateActions;
    pausedStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_S, std::bind(&AudiobookPlayer::togglePause, this)));
    pausedStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_UP, std::bind(&AudioManager::increaseMasterVolume, &audioManager_)));
    pausedStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_DOWN, std::bind(&AudioManager::decreaseMasterVolume, &audioManager_)));
    pausedStateActions.push_back(std::make_pair(InputManager::Button::BUTTON_F, std::bind(&AudiobookPlayer::stopAudiobook, this)));
    availableActions_.insert(std::make_pair(State::PAUSED, std::move(pausedStateActions)));
}

void AudiobookPlayer::playAudiobook(AudioTrack& audioTrack)
{
    BOOST_LOG_TRIVIAL(info) << "Playing audiotrack: " << audioTrack.getTrackName() << " (" << audioTrack.getFilePath() << ")";
    currentlyPlayedAudioStream_ = audioManager_.playMultipleAndGetLastAudioStream(std::vector<AudioTrack>({
            voiceManager_.getSynthesizedVoiceAudioTracks().at("playing_audiobook"),
            audioTrack
        })).get();
    auto audiobookFinishCallback = [this]()
    {
        currentlyPlayedAudioStream_->getAudioStreamFuture().get();
        changeStateTo(State::CHOOSING);
        BOOST_LOG_TRIVIAL(info) << "Finished playing audiotrack.";
        audioManager_.playAndGetAudioStream(voiceManager_.getSynthesizedVoiceAudioTracks().at("stopping_audiobook"));
    };
    std::thread(audiobookFinishCallback).detach();
}

void AudiobookPlayer::togglePause()
{
    BOOST_LOG_TRIVIAL(info) << "Toggling audiotrack pause: " << audioTracks_[currentTrackIndex_].getTrackName();
    fastForwardingSpeed_ = 0;
    if (currentlyPlayedAudioStream_->isPaused())
    {
        audioManager_.playSync(voiceManager_.getSynthesizedVoiceAudioTracks().at("unpausing_audiobook"));
        changeStateTo(State::PLAYING);
    }
    else
    {
        audioManager_.playAndGetAudioStream(voiceManager_.getSynthesizedVoiceAudioTracks().at("pausing_audiobook"));
        changeStateTo(State::PAUSED);
    }
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
                    AudioTrack audioTrack("../data/audiobooks/" + trackName, Config::getInstance().volumeForAudiobooks);
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

void AudiobookPlayer::rewind()
{
    if (fastForwardingSpeed_ == 0)
    {
        currentlyPlayedAudioStream_->pauseToggle();
        fastForwardingSpeed_ = -2;
        changeStateTo(State::REWINDING);
        std::thread([=]()
        {
            int fastForwardedSeconds = 0;
            while (fastForwardingSpeed_ != 0)
            { 
                auto sleepToTime = std::chrono::steady_clock::now() + std::chrono::seconds(1);
                fastForwardedSeconds += fastForwardingSpeed_;
                std::this_thread::sleep_until(sleepToTime);
                BOOST_LOG_TRIVIAL(info) << "Fast-forwarded seconds: " << fastForwardedSeconds;
            }
            currentlyPlayedAudioStream_->seek(fastForwardedSeconds);
        }).detach();
    }
    else if (fastForwardingSpeed_ == 2)
    {
        currentlyPlayedAudioStream_->pauseToggle();
        fastForwardingSpeed_ = 0;
        changeStateTo(State::PLAYING);
    }
    else if (fastForwardingSpeed_ < 0) fastForwardingSpeed_ *= 2;
    else if (fastForwardingSpeed_ > 0) fastForwardingSpeed_ *= 0.5f;

    BOOST_LOG_TRIVIAL(info) << "Set fast-forwardings speed to " << fastForwardingSpeed_;
}
void AudiobookPlayer::fastForward()
{
    if (fastForwardingSpeed_ == 0)
    {
        currentlyPlayedAudioStream_->pauseToggle();
        fastForwardingSpeed_ = 2;
        changeStateTo(State::FAST_FORWARDING);
        std::thread([=]()
        {
            int fastForwardedSeconds = 0;
            while (fastForwardingSpeed_ != 0)
            { 
                auto sleepToTime = std::chrono::steady_clock::now() + std::chrono::seconds(1);
                fastForwardedSeconds += fastForwardingSpeed_;
                std::this_thread::sleep_until(sleepToTime);
                BOOST_LOG_TRIVIAL(info) << "Fast-forwarded seconds: " << fastForwardedSeconds;
            }
            currentlyPlayedAudioStream_->seek(fastForwardedSeconds);
        }).detach();
    }
    else if (fastForwardingSpeed_ == -2)
    {
        currentlyPlayedAudioStream_->pauseToggle();
        fastForwardingSpeed_ = 0;
        changeStateTo(State::PLAYING);
    }
    else if (fastForwardingSpeed_ < 0) fastForwardingSpeed_ *= 0.5f;
    else if (fastForwardingSpeed_ > 0) fastForwardingSpeed_ *= 2;

    BOOST_LOG_TRIVIAL(info) << "Set fast-forwardings speed to " << fastForwardingSpeed_;
}
void AudiobookPlayer::stopAudiobook()
{
    changeStateTo(State::CHOOSING);
    audioManager_.playAndGetAudioStream(voiceManager_.getSynthesizedVoiceAudioTracks().at("stopping_audiobook"));
    BOOST_LOG_TRIVIAL(info) << "Audiobook stopped.";
}

void AudiobookPlayer::printState()
{
    BOOST_LOG_TRIVIAL(info) << static_cast<std::underlying_type<State>::type>(currentState_);
}

}