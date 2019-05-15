#include "AudioTask.h"
#include <boost/log/trivial.hpp>

namespace Pdb
{

AudioTask::Element::Element(AudioTrack& track) : taskTrack_(&track), taskStream_(nullptr)
{
}

AudioTask::Element::Element(AudioStream& existingStream) : taskTrack_(nullptr), taskStream_(&existingStream)
{
}

AudioTask::Element::~Element()
{
}

AudioTask::AudioTask() : state_(State::AVAILABLE)
{
}

AudioTask::~AudioTask()
{
    BOOST_LOG_TRIVIAL(info) << "Destroying AudioTask.";
}

void AudioTask::start(std::list<AudioTask::Element> taskElements, std::function<void()> callbackFunction)
{
    std::unique_lock<std::mutex> lock(mutex_);
    state_ = State::UNAVAILABLE;
    audioTaskElements_ = taskElements;
    taskCallbackFunction_ = callbackFunction;
    taskThread_ = std::thread(&AudioTask::taskFunction, this);
    taskThread_.detach();
}

void AudioTask::stop()
{
    std::unique_lock<std::mutex> lock(mutex_);
    if (state_ == State::AVAILABLE) return;
    taskCallbackFunction_ = {};
    auto taskElement = audioTaskElements_.front();
    AudioStream* currentStream = taskElement.getStream();
    for (auto& audioTaskElement : audioTaskElements_)
    {
        audioTaskElement.getStream()->stop();
    }
    audioTaskElements_.clear();
    if (currentStream) currentStream->stop();
}

bool AudioTask::isPausable() const
{
    std::unique_lock<std::mutex> lock(mutex_);
    if (audioTaskElements_.empty()) return false;
    auto taskElement = audioTaskElements_.front();
    AudioStream* currentStream = taskElement.getStream();
    if (currentStream && currentStream->isPausable()) return true;
    return false;
}

void AudioTask::pause()
{
    std::unique_lock<std::mutex> lock(mutex_);
    if (state_ == State::AVAILABLE) return;
    auto taskElement = audioTaskElements_.front();
    AudioStream* currentStream = taskElement.getStream();
    if (currentStream && currentStream->isPausable()) currentStream->pauseToggle();
}

void AudioTask::seek(int offsetInSeconds)
{
    std::unique_lock<std::mutex> lock(mutex_);
    if (state_ == State::AVAILABLE) return;
    auto taskElement = audioTaskElements_.front();
    AudioStream* currentStream = taskElement.getStream();
    if (currentStream && currentStream->isPausable()) currentStream->seek(offsetInSeconds);
}

void AudioTask::waitForEnd()
{
    std::unique_lock<std::mutex> lock(mutex_);
    taskFinishedCondVar_.wait(lock, [&] { return audioTaskElements_.empty(); });
    BOOST_LOG_TRIVIAL(info) << "Finished waiting for AudioStream end.";
}

void AudioTask::taskFunction()
{
    std::unique_lock<std::mutex> lock(mutex_);
    while (!audioTaskElements_.empty())
    {
        AudioTask::Element audioTaskElement = audioTaskElements_.front();
        audioTaskElement.getStream()->play();
        lock.unlock();
        audioTaskElement.getStream()->waitForEnd();
        lock.lock();
        if (audioTaskElements_.empty()) break;
        audioTaskElements_.pop_front();
    }
    if(taskCallbackFunction_) taskCallbackFunction_();
    state_ = State::AVAILABLE;
    taskFinishedCondVar_.notify_all();
}

}