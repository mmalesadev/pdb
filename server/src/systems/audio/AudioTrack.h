#pragma once
#include <string>
#include <memory>

namespace Pdb
{

class AudioTrack
{
public:
    AudioTrack(const std::string & filePath, const float masterVolume);
    ~AudioTrack();
    enum class Format { MP3, WAV };

    bool isMp3() const { return format_ == Format::MP3; }
    bool isWav() const { return format_ == Format::WAV; }
    std::string getFilePath() const { return filePath_; }
    std::string getTrackName() const { return trackName_; }
    float getMasterVolume() const { return masterVolume_; }

private:
    Format format_;
    std::string filePath_;
    float masterVolume_;    /* Values range from 0.0 to 1.0 */
    std::string trackName_;
};

}