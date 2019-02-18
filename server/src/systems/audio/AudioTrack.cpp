#include "AudioTrack.h"
#include <boost/log/trivial.hpp>

AudioTrack::AudioTrack(const std::string & fileName)
{
    std::string fileExtension = fileName.substr(fileName.length() - 3, 3);
    if (fileExtension == "mp3") format_ = Format::MP3;
    else if (fileExtension == "wav") format_ = Format::WAV;
    else
    {
        BOOST_LOG_TRIVIAL(error) << "Unknown file extension.";
        exit(0);
    }
    trackName_ = fileName.substr(0, fileName.length() - 4);
    fileName_ = fileName;
}

AudioTrack::~AudioTrack()
{

}
