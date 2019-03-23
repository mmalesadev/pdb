#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "systems/audio/AudioStream.h"
#include "systems/audio/AudioStreamMp3.h"
#include "systems/audio/AudioStreamWav.h"
#include "systems/audio/AudioTrack.h"

namespace Pdb
{

class AudioManager
{
public:
    AudioManager(const size_t nMp3AudioStreams = 7, const size_t nWavAudioStreams = 7);

    AudioStream* playAndGetAudioStream(const AudioTrack& audioTrack);
    std::future<AudioStream*> playMultipleAndGetLastAudioStream(const std::vector<AudioTrack>& audioTracks);

    size_t getMp3AudioStreamCount() const { return mp3AudioStreams_.size(); }
    int getFreeMp3AudioStreamCount() const;
    int getFreeWavAudioStreamCount() const;

    void printAllStreamsInfo() const;

private:
    std::vector< std::unique_ptr<AudioStreamMp3> > mp3AudioStreams_;
    std::vector< std::unique_ptr<AudioStreamWav> > wavAudioStreams_;
};

}