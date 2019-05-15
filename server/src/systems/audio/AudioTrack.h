#pragma once
#include <string>
#include <memory>

namespace Pdb
{

class AudioTrack
{
public:
    enum class Format { MP3, WAV };
    enum class Type { STANDARD, VOICE_MESSAGE };

    AudioTrack(const std::string & filePath, const float volume, Type type);
    ~AudioTrack();

    bool isMp3() const { return format_ == Format::MP3; }
    bool isWav() const { return format_ == Format::WAV; }
    bool isStandard() const { return type_ == Type::STANDARD; }
    bool isVoiceMessage() const { return type_ == Type::VOICE_MESSAGE; }
    std::string getFilePath() const { return filePath_; }
    std::string getTrackName() const { return trackName_; }
    float getVolume() const { return volume_; }

private:
    Format format_;
    Type type_;
    std::string filePath_;
    float volume_;    /* Values range from 0.0 to 1.0 */
    std::string trackName_;
};

}