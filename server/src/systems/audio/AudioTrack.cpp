#include "AudioTrack.h"
#include <boost/log/trivial.hpp>

namespace Pdb
{

AudioTrack::AudioTrack(std::string trackName, int lastPlayedMillisecond) : trackName_(trackName), lastPlayedMillisecond_(lastPlayedMillisecond)
{

}

AudioTrack::AudioTrack(const std::string & filePath, const float volume, Type type) : filePath_(filePath), volume_(volume), lastPlayedMillisecond_(0), type_(type)
{
    std::string fileExtension = filePath.substr(filePath.length() - 3, 3);
    if (fileExtension == "mp3") format_ = Format::MP3;
    else if (fileExtension == "wav") format_ = Format::WAV;
    else
    {
        BOOST_LOG_TRIVIAL(error) << "Unknown file extension.";
        exit(0);
    }
    size_t lastSlashIndex = filePath.find_last_of("/\\");
    std::string fileName = filePath.substr(lastSlashIndex + 1);
    trackName_ = fileName.substr(0, fileName.length() - 4);
    BOOST_LOG_TRIVIAL(debug) << "Audio track created: " << trackName_ << ", volume: " << volume_;
}

AudioTrack::~AudioTrack()
{
}

}