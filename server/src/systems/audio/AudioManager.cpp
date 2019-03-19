#include "AudioManager.h"
#include <boost/log/trivial.hpp>

namespace Pdb
{

AudioManager::AudioManager(const size_t nMp3AudioStreams, const size_t nWavAudioStreams)
{
    BOOST_LOG_TRIVIAL(info) << "Creating AudioManager app.";
    BOOST_LOG_TRIVIAL(info) << "Initializing " << nMp3AudioStreams << " mp3 audio streams and " << 
        nWavAudioStreams << " wav audio streams.";
    for (int i = 0; i < nMp3AudioStreams; ++i)
        mp3AudioStreams_.push_back(std::make_unique<AudioStreamMp3>());
    for (int i = 0; i < nWavAudioStreams; ++i)
       wavAudioStreams_.push_back(std::make_unique<AudioStreamWav>());
}

AudioStream* AudioManager::playAndGetAudioStream(const AudioTrack& audioTrack)
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

    BOOST_LOG_TRIVIAL(info) << "Playing " << audioTrack.getFilePath() << ".";
    if(foundStream) foundStream->play(audioTrack);
    return foundStream;
}

int AudioManager::getFreeMp3AudioStreamCount() const
{ 
    return std::count_if(mp3AudioStreams_.begin(), mp3AudioStreams_.end(), 
        [this](auto& stream){ return stream->isAvailable(); }
    );
}

}