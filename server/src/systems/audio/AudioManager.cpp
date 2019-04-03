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
}

AudioStream* AudioManager::playAndGetAudioStream(const AudioTrack& audioTrack)
{
    BOOST_LOG_TRIVIAL(info) << "Playing " << audioTrack.getFilePath() << ".";

    AudioStream* foundStream = findFreeStream(audioTrack);

    if(foundStream)
    {
        foundStream->play(audioTrack);
    }

    return foundStream;
}

void AudioManager::playSync(const AudioTrack& audioTrack)
{
    BOOST_LOG_TRIVIAL(info) << "Playing " << audioTrack.getFilePath() << ".";

    playAndGetAudioStream(audioTrack)->getAudioStreamFuture().get();
}

std::future<AudioStream*> AudioManager::playMultipleAndGetLastAudioStream(const std::vector<AudioTrack>& audioTracks)
{
    BOOST_LOG_TRIVIAL(info) << "Playing multiple audio tracks.";
    std::shared_ptr<std::promise<AudioStream*>> lastAudioStream = std::make_shared<std::promise<AudioStream*>>();

    auto audioStreamChainPlayFunction = [this](const std::vector<AudioTrack> audioTracks, std::shared_ptr<std::promise<AudioStream*>> lastAudioStream)
    {
        size_t nAudioTracks = audioTracks.size();
        for (int i = 0; i < nAudioTracks-1; ++i)
        {
            playAndGetAudioStream(audioTracks[i])->getAudioStreamFuture().get();
        }
        lastAudioStream->set_value(playAndGetAudioStream(audioTracks[nAudioTracks-1]));
    };

    std::thread(audioStreamChainPlayFunction, audioTracks, lastAudioStream).detach();

    return lastAudioStream->get_future();
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
    if (masterVolume_ <= 0.9)
        masterVolume_ += 0.1;
}

void AudioManager::decreaseMasterVolume()
{
    if (masterVolume_ >= 0.1)
        masterVolume_ -= 0.1;
}

void AudioManager::printAllStreamsInfo() const
{
    for (auto& stream : mp3AudioStreams_)
    {
        BOOST_LOG_TRIVIAL(debug) << "mp3 stream isAvailable=" << stream->isAvailable();
    }
    for (auto& stream : wavAudioStreams_)
    {
        BOOST_LOG_TRIVIAL(debug) << "wav stream isAvailable=" << stream->isAvailable();
    }
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

    if(!foundStream)
    {
        BOOST_LOG_TRIVIAL(error) << "No free audio stream found. Current free mp3 audio stream count: " << getFreeMp3AudioStreamCount()
            << ". Current free wav audio stream count: " << getFreeWavAudioStreamCount();
    }
    return foundStream;
}

}