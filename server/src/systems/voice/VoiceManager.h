#pragma once
#include <string>
#include <fstream>
#include <unordered_map>

#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/core/client/AWSClient.h>
#include <aws/core/utils/Outcome.h>
#include <aws/core/utils/stream/ResponseStream.h>

#include <aws/polly/PollyClient.h>
#include <aws/polly/model/SynthesizeSpeechRequest.h>
#include <aws/polly/model/SynthesizeSpeechResult.h>
#include <aws/polly/PollyRequest.h>

#include "Config.h"

#include "systems/audio/AudioTrack.h"

namespace Pdb
{
    
class VoiceManager
{
public:
    VoiceManager();
    ~VoiceManager();

    void synthesizeVoiceMessage(const std::string& message, const std::string& outputDirectory, const std::string& outputTrackName);
    std::unordered_map<std::string, AudioTrack>& getSynthesizedVoiceAudioTracks() { return synthesizedVoiceAudioTracks_; }

private:
    int GetStreamSize(Aws::IOStream* stream);
    char* GetStreamBytes(Aws::IOStream* stream);

    Aws::SDKOptions options_;

    std::unordered_map<std::string, AudioTrack> synthesizedVoiceAudioTracks_;
};

}