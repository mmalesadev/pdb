#include "VoiceManager.h"
#include <boost/log/trivial.hpp>
#include <boost/filesystem.hpp>

#include <utility>

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

void VoiceManager::synthesizeVoiceMessage(const std::string& message, const std::string& outputDirectory, const std::string& outputTrackName)
{
    boost::filesystem::path synthesizedVoiceMessageFilePath(outputDirectory + "/" + outputTrackName + ".mp3");
    if (boost::filesystem::exists(synthesizedVoiceMessageFilePath))
    {
        BOOST_LOG_TRIVIAL(info) << "Voice message already synthesized (" << synthesizedVoiceMessageFilePath << ").";
        synthesizedVoiceAudioTracks_.insert(std::make_pair(outputTrackName, AudioTrack(synthesizedVoiceMessageFilePath.c_str())));
        return;
    }

    BOOST_LOG_TRIVIAL(info) << "Synthesizing a voice message: " << message << " (path: " << synthesizedVoiceMessageFilePath.c_str() << ").";

    Aws::Polly::PollyClient pollyClient;
    Aws::Polly::Model::SynthesizeSpeechRequest speechRequest;
    speechRequest.SetTextType(Aws::Polly::Model::TextType::ssml);
    speechRequest.SetVoiceId(Aws::Polly::Model::VoiceId::Maja);
    speechRequest.SetOutputFormat(Aws::Polly::Model::OutputFormat::mp3);
    speechRequest.SetText(message.c_str());
    auto result = pollyClient.SynthesizeSpeech(speechRequest);

    if (result.IsSuccess())
	{
        BOOST_LOG_TRIVIAL(info) << "Speech synthesis was successfull. Saving to file " << synthesizedVoiceMessageFilePath;

        Aws::String     filePath = synthesizedVoiceMessageFilePath.c_str();
        Aws::IOStream*  audioStream = &result.GetResult().GetAudioStream();
        Aws::OFStream   voiceFile;
        voiceFile.open(filePath.c_str(), std::ios::out | std::ios::binary);
        voiceFile.write(GetStreamBytes(audioStream), GetStreamSize(audioStream));
        voiceFile.close();
        synthesizedVoiceAudioTracks_.insert(std::make_pair(outputTrackName, AudioTrack(synthesizedVoiceMessageFilePath.c_str())));

        BOOST_LOG_TRIVIAL(info) << "Saving to file done.";
    }
    else
    {
        BOOST_LOG_TRIVIAL(error) << "Speech synthesis failed. Error: " << result.GetError().GetMessage();
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