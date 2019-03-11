#include "VoiceManager.h"
#include <boost/log/trivial.hpp>

namespace Pdb
{

VoiceManager::VoiceManager()
{
	BOOST_LOG_TRIVIAL(info) << "Initializing VoiceManager";
    Aws::InitAPI(options_);
}

VoiceManager::~VoiceManager()
{
    Aws::ShutdownAPI(options_);
}

void VoiceManager::synthesizeVoiceMessage(const std::string& message, const std::string& outputDirectory, const std::string& outputFileName)
{
    BOOST_LOG_TRIVIAL(info) << "Synthesizing a voice message: " << message;

    Aws::Polly::PollyClient pollyClient;
    Aws::Polly::Model::SynthesizeSpeechRequest speechRequest;

    speechRequest.SetVoiceId(Aws::Polly::Model::VoiceId::Maja);
    speechRequest.SetOutputFormat(Aws::Polly::Model::OutputFormat::mp3);

    speechRequest.SetText(message.c_str());
    auto result = pollyClient.SynthesizeSpeech(speechRequest);

    if (result.IsSuccess())
	{
        std::string fileNameStr = outputDirectory + "/" + outputFileName + ".mp3";
        BOOST_LOG_TRIVIAL(info) << "Speech synthesis was successfull. Saving to file " << fileNameStr;

        Aws::String     fileName = fileNameStr.c_str();
        Aws::IOStream*  audioStream = &result.GetResult().GetAudioStream();
        Aws::OFStream   voiceFile;

        // Get the bytes from the stream and save them into a file
        voiceFile.open(fileName.c_str(), std::ios::out | std::ios::binary);
        voiceFile.write(GetStreamBytes(audioStream), GetStreamSize(audioStream));
        voiceFile.close();

        BOOST_LOG_TRIVIAL(info) << "Done!";
    }
    else{
        BOOST_LOG_TRIVIAL(error) << "Speech synthesis failed.";
        BOOST_LOG_TRIVIAL(error) << "Error: " << result.GetError().GetMessage();
	}

}

int VoiceManager::GetStreamSize(Aws::IOStream* stream)
{
    // Ensure the stream is at the beginning
                stream->seekg (0, std::ios::beg);
    int begin = stream->tellg();
                stream->seekg (0, std::ios::end);
    int end =   stream->tellg();
                stream->seekg (0, std::ios::beg);

    return end-begin;
}

char* VoiceManager::GetStreamBytes(Aws::IOStream* stream)
{
    // Read the stream bytes into a memory block
    int size = GetStreamSize(stream);
    char* bytes = new char[size];
    stream->read(bytes, size);

    return bytes;
}


}