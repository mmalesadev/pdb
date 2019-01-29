#ifndef _AUDIO_TRACK_
#define _AUDIO_TRACK_
#include <string>
#include <memory>

class AudioTrack
{
public:
    AudioTrack(const std::string & title);
    ~AudioTrack();

    std::string getTitle() { return title_; };

private:
    std::string title_;

};


#endif




