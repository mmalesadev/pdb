#pragma once
#include "PDBApp.h"
#include "systems/audio/AudioManager.h"
#include "audiobook/AudiobookPlayer.h"
#include <cstring>

class PDBAudiobook : public PDBApp
{
public:
    PDBAudiobook(AudioManager& audioManager);
    void init();
    void appLoopFunction();
private:
    void playMessage(const std::string & message);

    AudiobookPlayer audiobookPlayer_;
};