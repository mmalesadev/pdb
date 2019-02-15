#include "PDBAudiobook.h"
#include "spdlog/spdlog.h"

PDBAudiobook::PDBAudiobook(AudioManager& audioManager) : audiobookPlayer_(audioManager)
{
    spdlog::get("console")->info("Creating PDBAudiobook app.");
}

void PDBAudiobook::init()
{
    spdlog::get("console")->info("Initialized PDBAudiobook.");
}

void PDBAudiobook::appLoopFunction()
{
    spdlog::get("console")->info("Starting PDBAudiobook loop function.");
    AudioTrack audioTrack("klapsczang.wav");
    audiobookPlayer_.playAudiobook(audioTrack);

    while(true)
    {

    }
    spdlog::get("console")->info("Ending PDBAudiobook loop function.");
}

void playMessage(const std::string & message)
{
    std::string milenaMessageCall = "milena_say " + message;
    system(milenaMessageCall.c_str());
}