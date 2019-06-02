#include "AudiobookPlayer.h"
#include <boost/log/trivial.hpp>
#include <future>
#include <memory>
#include <boost/filesystem.hpp>

namespace filesystem = boost::filesystem;

namespace Pdb
{

AudiobookPlayer::AudiobookPlayer(AudioManager& audioManager, VoiceManager& voiceManager) 
    : audioManager_(audioManager), voiceManager_(voiceManager), fastForwardingSpeed_(0), currentAudioTask_(nullptr), pausedAudioTask_(nullptr)
{
    this->loadTracks();
    currentTrackIndex_ = 0;
    currentState_ = State::CHOOSING;

    // CHOOSING STATE
    std::vector< std::pair<InputManager::Button, std::function<void()> > > choosingStateActions;
    choosingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpLeft, std::bind(&AudiobookPlayer::switchToPreviousAudiobook, this)));
    choosingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpBegin, std::bind(&AudiobookPlayer::playChosenAudiobook, this)));
    choosingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpRight, std::bind(&AudiobookPlayer::switchToNextAudiobook, this)));
    choosingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpAdd, std::bind(&AudioManager::increaseMasterVolume, &audioManager_)));
    choosingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpSubtract, std::bind(&AudioManager::decreaseMasterVolume, &audioManager_)));
    availableActions_.insert(std::make_pair(State::CHOOSING, std::move(choosingStateActions)));

    // PLAYING STATE
    std::vector< std::pair<InputManager::Button, std::function<void()> > > playingStateActions;
    playingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpLeft, std::bind(&AudiobookPlayer::rewind, this)));
    playingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpRight, std::bind(&AudiobookPlayer::fastForward, this)));
    playingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpBegin, std::bind(&AudiobookPlayer::pauseToggle, this)));
    playingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpAdd, std::bind(&AudioManager::increaseMasterVolume, &audioManager_)));
    playingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpSubtract, std::bind(&AudioManager::decreaseMasterVolume, &audioManager_)));
    playingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpInsert, std::bind(&AudiobookPlayer::stopAudiobook, this)));
    availableActions_.insert(std::make_pair(State::PLAYING, std::move(playingStateActions)));

    // REWINDING STATE
    std::vector< std::pair<InputManager::Button, std::function<void()> > > rewindingStateActions;
    rewindingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpLeft, std::bind(&AudiobookPlayer::rewind, this)));
    rewindingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpRight, std::bind(&AudiobookPlayer::fastForward, this)));
    rewindingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpBegin, std::bind(&AudiobookPlayer::pauseToggle, this)));
    rewindingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpAdd, std::bind(&AudioManager::increaseMasterVolume, &audioManager_)));
    rewindingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpSubtract, std::bind(&AudioManager::decreaseMasterVolume, &audioManager_)));
    availableActions_.insert(std::make_pair(State::REWINDING, std::move(rewindingStateActions)));

    // FAST_FORWARDING STATE
    std::vector< std::pair<InputManager::Button, std::function<void()> > > fastForwardingStateActions;
    fastForwardingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpLeft, std::bind(&AudiobookPlayer::rewind, this)));
    fastForwardingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpRight, std::bind(&AudiobookPlayer::fastForward, this)));
    fastForwardingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpBegin, std::bind(&AudiobookPlayer::pauseToggle, this)));
    fastForwardingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpAdd, std::bind(&AudioManager::increaseMasterVolume, &audioManager_)));
    fastForwardingStateActions.push_back(std::make_pair(InputManager::Button::KeyKpSubtract, std::bind(&AudioManager::decreaseMasterVolume, &audioManager_)));
    availableActions_.insert(std::make_pair(State::FAST_FORWARDING, std::move(fastForwardingStateActions)));

    // PAUSED STATE
    std::vector< std::pair<InputManager::Button, std::function<void()> > > pausedStateActions;
    pausedStateActions.push_back(std::make_pair(InputManager::Button::KeyKpLeft, std::bind(&AudiobookPlayer::rewind, this)));
    pausedStateActions.push_back(std::make_pair(InputManager::Button::KeyKpRight, std::bind(&AudiobookPlayer::fastForward, this)));
    pausedStateActions.push_back(std::make_pair(InputManager::Button::KeyKpBegin, std::bind(&AudiobookPlayer::pauseToggle, this)));
    pausedStateActions.push_back(std::make_pair(InputManager::Button::KeyKpAdd, std::bind(&AudioManager::increaseMasterVolume, &audioManager_)));
    pausedStateActions.push_back(std::make_pair(InputManager::Button::KeyKpSubtract, std::bind(&AudioManager::decreaseMasterVolume, &audioManager_)));
    pausedStateActions.push_back(std::make_pair(InputManager::Button::KeyKpInsert, std::bind(&AudiobookPlayer::stopAudiobook, this)));
    availableActions_.insert(std::make_pair(State::PAUSED, std::move(pausedStateActions)));
}

void AudiobookPlayer::play(std::list<AudioTask::Element> audioTaskElements, std::function<void()> callbackFunction)
{
    currentAudioTask_ = audioManager_.play(audioTaskElements, callbackFunction);
}

void AudiobookPlayer::playChosenAudiobook()
{
    std::lock_guard<std::mutex> lock(mutex_);
    changeStateTo(State::PLAYING);
    AudioTrack& currentAudioTrack = audioTracks_[currentTrackIndex_];

    BOOST_LOG_TRIVIAL(info) << "Playing audiotrack: " << currentAudioTrack.getTrackName() << " (" << currentAudioTrack.getFilePath() << ")";

    if (currentAudioTask_) currentAudioTask_->stop();
    
    auto audiobookFinishCallback = [this]()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (currentState_ == State::PLAYING)
        {
            changeStateTo(State::CHOOSING);
            BOOST_LOG_TRIVIAL(info) << "Finished playing audiotrack.";
            play({ voiceManager_.getSynthesizedVoiceAudioTracks().at("stopping_audiobook") });
        }
    };

    play({ voiceManager_.getSynthesizedVoiceAudioTracks().at("playing_audiobook"), currentAudioTrack },
        audiobookFinishCallback);
}

void AudiobookPlayer::fastForwardingTimerFunction()
{
    fastForwardedSeconds_ = 0;
    while (fastForwardingSpeed_ != 0)
    { 
        auto sleepToTime = std::chrono::steady_clock::now() + std::chrono::seconds(1);
        fastForwardedSeconds_ += fastForwardingSpeed_;
        std::this_thread::sleep_until(sleepToTime);
        BOOST_LOG_TRIVIAL(info) << "Fast-forwarded seconds: " << fastForwardedSeconds_;
    }
}

void AudiobookPlayer::pauseToggle()
{
    if (currentState_ == State::PAUSED || currentState_ == State::FAST_FORWARDING || currentState_ == State::REWINDING)
    {
        if (!pausedAudioTask_) return;
        if (!pausedAudioTask_->isPausable()) return;
        BOOST_LOG_TRIVIAL(info) << "Toggling audiotrack pause: " << audioTracks_[currentTrackIndex_].getTrackName();
        fastForwardingSpeed_ = 0;
        if (fastForwardingTimerThread_.joinable()) fastForwardingTimerThread_.join();
        changeStateTo(State::PLAYING);
        pausedAudioTask_->seek(fastForwardedSeconds_);
        fastForwardedSeconds_ = 0;
        play({ voiceManager_.getSynthesizedVoiceAudioTracks().at("unpausing_audiobook") });
        currentAudioTask_->waitForEnd();
        if (pausedAudioTask_)
        {
            pausedAudioTask_->pause();
            currentAudioTask_ = pausedAudioTask_;
        }
        pausedAudioTask_ = nullptr;
    }
    else
    {
        if (!currentAudioTask_) return;
        if (!currentAudioTask_->isPausable()) return;
        BOOST_LOG_TRIVIAL(info) << "Toggling audiotrack pause: " << audioTracks_[currentTrackIndex_].getTrackName();
        fastForwardingSpeed_ = 0;
        if (fastForwardingTimerThread_.joinable()) fastForwardingTimerThread_.join();
        changeStateTo(State::PAUSED);
        currentAudioTask_->seek(fastForwardedSeconds_);
        fastForwardedSeconds_ = 0;
        if (currentAudioTask_) currentAudioTask_->pause();
        pausedAudioTask_ = currentAudioTask_;
        play({ voiceManager_.getSynthesizedVoiceAudioTracks().at("pausing_audiobook") });
    }
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
                    AudioTrack audioTrack("../data/audiobooks/" + trackName, Config::getInstance().volumeForAudiobooks, AudioTrack::Type::STANDARD);
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
    std::lock_guard<std::mutex> lock(mutex_);
    if (currentTrackIndex_ == audioTracks_.size() - 1)
        currentTrackIndex_ = 0;
    else
        ++currentTrackIndex_;

    if (currentAudioTask_) currentAudioTask_->stop();
    play({ voiceManager_.getSynthesizedVoiceAudioTracks().at("chosen_next"), 
        voiceManager_.getSynthesizedVoiceAudioTracks().at(getCurrentTrack().getTrackName()) 
        });
}

void AudiobookPlayer::switchToPreviousAudiobook()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (currentTrackIndex_ == 0)
        currentTrackIndex_ = audioTracks_.size() - 1;  
    else
        --currentTrackIndex_;

    if (currentAudioTask_) currentAudioTask_->stop();
    play({ voiceManager_.getSynthesizedVoiceAudioTracks().at("chosen_previous"), 
        voiceManager_.getSynthesizedVoiceAudioTracks().at(getCurrentTrack().getTrackName()) 
        });
}

std::vector< std::pair<InputManager::Button, std::function<void()> > > & AudiobookPlayer::getAvailableActions()
{
    return availableActions_.at(currentState_);
}

void AudiobookPlayer::rewind()
{
    std::unique_lock<std::mutex> lock(mutex_);
    AudioTask* checkedAudioTask = nullptr;
    if (currentState_ == State::PLAYING) checkedAudioTask = currentAudioTask_;
    else if (currentState_ == State::PAUSED) checkedAudioTask = pausedAudioTask_;

    if (fastForwardingSpeed_ == 0)
    {
        if (!checkedAudioTask) return;
        if (!checkedAudioTask->isPausable()) return;
        pausedAudioTask_ = currentAudioTask_;
        if (!checkedAudioTask->isPaused())
        {
            pausedAudioTask_ = currentAudioTask_;checkedAudioTask->pause();
        }
        fastForwardingSpeed_ = -2;
        changeStateTo(State::REWINDING);
        if (fastForwardingTimerThread_.joinable()) fastForwardingTimerThread_.join();
        fastForwardingTimerThread_ = std::thread(&AudiobookPlayer::fastForwardingTimerFunction, this);
    }
    else if (fastForwardingSpeed_ == 2)
    {
        fastForwardingSpeed_ = 0;
        currentAudioTask_ = pausedAudioTask_;
        currentAudioTask_->pause();
        changeStateTo(State::PLAYING);
    }
    else if (fastForwardingSpeed_ < 0) fastForwardingSpeed_ *= 2;
    else if (fastForwardingSpeed_ > 0) fastForwardingSpeed_ *= 0.5f;

    /* lower value */
    if (fastForwardingSpeed_ < -128) fastForwardingSpeed_ = -128;

    /* Playing appropriate voice messages */
    if (fastForwardingSpeed_ == -2)
    {
        play({ voiceManager_.getSynthesizedVoiceAudioTracks().at("rewinding"),
            voiceManager_.getSynthesizedVoiceAudioTracks().at("2x") });
    }
    else if (fastForwardingSpeed_ != 0)
    {
        play({ voiceManager_.getSynthesizedVoiceAudioTracks().at(std::to_string(std::abs(fastForwardingSpeed_)) + "x") });
    }

    BOOST_LOG_TRIVIAL(info) << "Set fast-forwardings speed to " << fastForwardingSpeed_;
}
void AudiobookPlayer::fastForward()
{
    std::unique_lock<std::mutex> lock(mutex_);
    AudioTask* checkedAudioTask = nullptr;
    if (currentState_ == State::PLAYING) checkedAudioTask = currentAudioTask_;
    else if (currentState_ == State::PAUSED) checkedAudioTask = pausedAudioTask_;

    if (fastForwardingSpeed_ == 0)
    {
        if (!checkedAudioTask) return;
        if (!checkedAudioTask->isPausable()) return;
        if (!checkedAudioTask->isPaused())
        {
            pausedAudioTask_ = currentAudioTask_;
            checkedAudioTask->pause();
        }
        fastForwardingSpeed_ = 2;
        changeStateTo(State::FAST_FORWARDING);
        if (fastForwardingTimerThread_.joinable()) fastForwardingTimerThread_.join();
        fastForwardingTimerThread_ = std::thread(&AudiobookPlayer::fastForwardingTimerFunction, this);
    }
    else if (fastForwardingSpeed_ == -2)
    {
        fastForwardingSpeed_ = 0;
        currentAudioTask_ = pausedAudioTask_;
        currentAudioTask_->pause();
        changeStateTo(State::PLAYING);
    }
    else if (fastForwardingSpeed_ < 0) fastForwardingSpeed_ *= 0.5f;
    else if (fastForwardingSpeed_ > 0) fastForwardingSpeed_ *= 2;

    /* Upper value */
    if (fastForwardingSpeed_ > 128) fastForwardingSpeed_ = 128;

    /* Playing appropriate voice messages */
    if (fastForwardingSpeed_ == 2)
    {
        play({ voiceManager_.getSynthesizedVoiceAudioTracks().at("fast_forwarding"),
            voiceManager_.getSynthesizedVoiceAudioTracks().at("2x") });
    }
    else if (fastForwardingSpeed_ != 0)
    {
        play({ voiceManager_.getSynthesizedVoiceAudioTracks().at(std::to_string(std::abs(fastForwardingSpeed_)) + "x") });
    }

    BOOST_LOG_TRIVIAL(info) << "Set fast-forwardings speed to " << fastForwardingSpeed_;
}
void AudiobookPlayer::stopAudiobook()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (currentAudioTask_) currentAudioTask_->stop();
    play( {voiceManager_.getSynthesizedVoiceAudioTracks().at("stopping_audiobook")} );
    BOOST_LOG_TRIVIAL(info) << "Audiobook stopped.";
    currentState_ = State::CHOOSING;
}

void AudiobookPlayer::printState()
{
    BOOST_LOG_TRIVIAL(info) << static_cast<std::underlying_type<State>::type>(currentState_);
}

}