#pragma once
#include "AudioStream.h"

#include <thread>
#include <mutex>
#include <list>
#include <functional>

namespace Pdb
{

class AudioTask
{
public:
    class Element
    {
        public:
            Element(AudioTrack& track);
            Element(AudioStream& existingStream);
            ~Element();

            bool hasStream() const { return taskStream_ != nullptr; }
            bool hasTrack() const { return taskTrack_ != nullptr; }
            void setStream(AudioStream* taskStream) { taskStream->reserve(); taskStream_ = taskStream; }
            AudioStream* getStream() const { return taskStream_; }
            AudioTrack* getTrack() const { return taskTrack_; }

        private:
            AudioTrack* taskTrack_;
            AudioStream* taskStream_;
    };

public:
    AudioTask();
    ~AudioTask();

    bool isAvailable() const { return state_ == State::AVAILABLE; };
    bool isPausable() const;
    bool isPaused() const;
    void start(std::list<AudioTask::Element> taskElements, std::function<void()> callbackFunction = {});
    void stop();
    void pause();
    void seek(int offsetInSeconds);
    void waitForEnd();

private:
    enum class State { AVAILABLE, UNAVAILABLE };
    void taskFunction();

    std::list<AudioTask::Element> audioTaskElements_;

    State state_;

    std::thread taskThread_;
    mutable std::mutex mutex_;
    std::condition_variable taskFinishedCondVar_;

    std::function<void()> taskCallbackFunction_;
};

}