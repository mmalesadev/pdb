#pragma once

#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <mutex>
#include <atomic>

#include "systems/audio/AudioStream.h"
#include "systems/audio/AudioStreamMp3.h"
#include "systems/audio/AudioStreamWav.h"
#include "systems/audio/AudioTrack.h"
#include "systems/audio/AudioTask.h"

namespace Pdb
{

class AudioManager
{
public:
    AudioManager(const size_t nMp3AudioStreams = 7, const size_t nWavAudioStreams = 0);


    AudioTask* play(std::list<AudioTask::Element> audioTaskElements, std::function<void()> callbackFunction = {});

    size_t getMp3AudioStreamCount() const { return mp3AudioStreams_.size(); }
    int getFreeMp3AudioStreamCount() const;
    int getFreeWavAudioStreamCount() const;

    void increaseMasterVolume();
    void decreaseMasterVolume();

    void printAllStreamsInfo() const;

private:
    AudioStream* findFreeStream(const AudioTrack& audioTrack);
    AudioTask* getFreeAudioTaskFromPool() const;

    std::vector<std::unique_ptr<AudioTask>> audioTaskPool_;
    std::vector< std::unique_ptr<AudioStreamMp3> > mp3AudioStreams_;
    std::vector< std::unique_ptr<AudioStreamWav> > wavAudioStreams_;

    std::atomic<float> masterVolume_;
    std::mutex mutex_;
};

}