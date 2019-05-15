#include "AudioManager.h"
#include <boost/log/trivial.hpp>
#include <thread>

#include "Config.h"

#include <future>
#include <memory>

namespace Pdb
{

AudioManager::AudioManager(const size_t nMp3AudioStreams, const size_t nWavAudioStreams) : masterVolume_(Config::getInstance().masterVolume)
{
    BOOST_LOG_TRIVIAL(info) << "Creating AudioManager app. Initializing " << nMp3AudioStreams << " mp3 audio streams and " << 
        nWavAudioStreams << " wav audio streams.";
    for (int i = 0; i < nMp3AudioStreams; ++i)
        mp3AudioStreams_.push_back(std::make_unique<AudioStreamMp3>(masterVolume_));
    for (int i = 0; i < nWavAudioStreams; ++i)
        wavAudioStreams_.push_back(std::make_unique<AudioStreamWav>(masterVolume_));

    for (int i = 0; i < nMp3AudioStreams + nWavAudioStreams; ++i)
        audioTaskPool_.push_back(std::make_unique<AudioTask>());
}

AudioTask* AudioManager::play(std::list<AudioTask::Element> audioTaskElements, std::function<void()> callbackFunction)
{
    std::lock_guard<std::mutex> lock(mutex_);

    AudioTask* freeAudioTask = getFreeAudioTaskFromPool();
    if (!freeAudioTask) return nullptr;

    AudioStream* freeStream = nullptr;

    for (auto& audioTaskElement : audioTaskElements)
    {
        if (audioTaskElement.hasTrack())
        {
            freeStream = findFreeStream(*audioTaskElement.getTrack());
            if (!freeStream) break;
            freeStream->setPlayedAudioTrack(audioTaskElement.getTrack());
            audioTaskElement.setStream(freeStream);
        }
    }

    if (freeStream)
    {
        freeAudioTask->start(audioTaskElements, callbackFunction);
        return freeAudioTask;
    }
    else    // cleanup
    {
        for (auto& audioTaskElement : audioTaskElements)
        {
            if (audioTaskElement.hasStream())
            {
                audioTaskElement.getStream()->makeAvailable();
            }
        }
        return nullptr;
    }
}

int AudioManager::getFreeWavAudioStreamCount() const
{ 
    return std::count_if(wavAudioStreams_.begin(), wavAudioStreams_.end(), 
        [this](auto& stream){ return stream->isAvailable(); }
    );
}

int AudioManager::getFreeMp3AudioStreamCount() const
{ 
    return std::count_if(mp3AudioStreams_.begin(), mp3AudioStreams_.end(), 
        [this](auto& stream){ return stream->isAvailable(); }
    );
}

void AudioManager::increaseMasterVolume()
{
    if (masterVolume_ <= 0.9f)
        masterVolume_.store(masterVolume_ + 0.1f);
}

void AudioManager::decreaseMasterVolume()
{
    if (masterVolume_ >= 0.1f)
        masterVolume_.store(masterVolume_ - 0.1f);
}

void AudioManager::printAllStreamsInfo() const
{
    for (auto& stream : mp3AudioStreams_)
    {
        BOOST_LOG_TRIVIAL(info) << "mp3 stream isAvailable=" << stream->isAvailable() << " " << stream->getPlayedAudioTrackName();
    }
    for (auto& stream : wavAudioStreams_)
    {
        BOOST_LOG_TRIVIAL(info) << "wav stream isAvailable=" << stream->isAvailable();
    }
}


AudioTask* AudioManager::getFreeAudioTaskFromPool() const
{
    AudioTask* foundTask = nullptr;
    for (auto& audioTask : audioTaskPool_)
    {
        if (audioTask->isAvailable())
        {
            foundTask = audioTask.get();
            break;
        }
    }

    if (!foundTask)
    {
        BOOST_LOG_TRIVIAL(error) << "No AudioTasks left in pool.";
    }
    return foundTask;
}

AudioStream* AudioManager::findFreeStream(const AudioTrack& audioTrack)
{
    AudioStream* foundStream = nullptr;
    if (audioTrack.isMp3())
    {
        for (auto& stream : mp3AudioStreams_) 
            if (stream->isAvailable()) 
                { foundStream = stream.get(); break; }
    }
    else if (audioTrack.isWav())
    {
        for (auto& stream : wavAudioStreams_) 
            if (stream->isAvailable())
                { foundStream = stream.get(); break; }
    }
    else
    {
        BOOST_LOG_TRIVIAL(error) << "Unknown audio track format. Aborting. Returning empty stream. " << audioTrack.getFilePath();
        return foundStream;
    }

    if (foundStream)
    {
        foundStream->reserve();
    }
    else
    {
        BOOST_LOG_TRIVIAL(error) << "No free audio stream found. Current free mp3 audio stream count: " << getFreeMp3AudioStreamCount()
            << ". Current free wav audio stream count: " << getFreeWavAudioStreamCount();
    }
    return foundStream;
}

}