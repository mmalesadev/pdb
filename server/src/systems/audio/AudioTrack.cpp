#include "AudioTrack.h"
#include "spdlog/spdlog.h"

AudioTrack::AudioTrack(const std::string & fileName)
{
    std::string fileExtension = fileName.substr(fileName.length() - 3, 3);
    if (fileExtension == "mp3") format_ = Format::MP3;
    else if (fileExtension == "wav") format_ = Format::WAV;
    else
    {
        spdlog::get("console")->error("Unknown file extension");
        exit(0);
    }
    trackName_ = fileName.substr(0, fileName.length() - 4);
    fileName_ = fileName;
}

AudioTrack::~AudioTrack()
{

}
