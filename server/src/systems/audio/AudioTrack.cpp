#include "AudioTrack.h"
#include <boost/log/trivial.hpp>

namespace Pdb
{

AudioTrack::AudioTrack(const std::string & filePath) : filePath_(filePath)
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
}

AudioTrack::~AudioTrack()
{

}

void AudioTrack::assignWith(const AudioTrack & track)
{
    this->filePath_ = track.filePath_;
    this->trackName_ = track.trackName_;
    this->format_ = track.format_;
}

}