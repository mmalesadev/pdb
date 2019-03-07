#pragma once
#include <string>
#include <memory>

namespace Pdb
{

class AudioTrack
{
public:
    AudioTrack(const std::string & fileName);
    ~AudioTrack();
    enum class Format { MP3, WAV };

    bool isMp3() const { return format_ == Format::MP3; }
    bool isWav() const { return format_ == Format::WAV; }
    std::string getTrackName() const { return trackName_; }
    std::string getFileName() const { return fileName_; }

private:
    Format format_;
    std::string trackName_;
    std::string fileName_;
};

}