#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "systems/audio/AudioStream.h"
#include "systems/audio/AudioStreamMp3.h"
#include "systems/audio/AudioStreamWav.h"
#include "systems/audio/AudioTrack.h"

class AudioManager
{
public:
    AudioManager(const size_t nMp3AudioStreams = 8, const size_t nWavAudioStreams = 8);

    AudioStream* playAndGetAudioStream(const AudioTrack& audioTrack);

    size_t getMp3AudioStreamCount() const { return mp3AudioStreams_.size(); }
    int getFreeMp3AudioStreamCount() const;

private:
    std::vector< std::unique_ptr<AudioStreamMp3> > mp3AudioStreams_;
    std::vector< std::unique_ptr<AudioStreamWav> > wavAudioStreams_;
};