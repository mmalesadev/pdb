#ifndef _AUDIO_PLAYER_
#define _AUDIO_PLAYER_

#include "AudioTrack.h"
#include <vector>

class AudioPlayer
{
public:
    AudioPlayer();
    ~AudioPlayer();

    void run(AudioTrack & audioTrack);

private:

    void init();
    void play(AudioTrack & audioTrack);
    void destroy();

    std::vector<AudioTrack> audioTracks_;

};


#endif
