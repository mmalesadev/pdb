#pragma once
#include <string>
#include <fstream>

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

namespace Pdb
{
    
class VoiceManager
{
public:
    VoiceManager();
    ~VoiceManager();

    void synthesizeVoiceMessage(const std::string& message, const std::string& outputDirectory, const std::string& outputFileName);

private:
    int GetStreamSize(Aws::IOStream* stream);
    char* GetStreamBytes(Aws::IOStream* stream);

    Aws::SDKOptions options_;
};

}