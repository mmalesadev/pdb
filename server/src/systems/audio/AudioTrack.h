#pragma once
#include <string>
#include <memory>

namespace Pdb
{

class AudioTrack
{
public:
    AudioTrack(const std::string & filePath);
    ~AudioTrack();
    enum class Format { MP3, WAV };

    bool isMp3() const { return format_ == Format::MP3; }
    bool isWav() const { return format_ == Format::WAV; }
    std::string getFilePath() const { return filePath_; }
    std::string getTrackName() const { return trackName_; }

private:
    Format format_;
    std::string filePath_;
    std::string trackName_;
};

}