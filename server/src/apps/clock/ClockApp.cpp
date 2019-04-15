#include "ClockApp.h"
#include <boost/log/trivial.hpp>
#include <ctime>



namespace Pdb
{

ClockApp::ClockApp(VoiceManager& voiceManager)
    : Pdb::App(voiceManager)
{
}

void ClockApp::init()
{
    this->synthesizeClockReadings();
    BOOST_LOG_TRIVIAL(info) << "Initialized Clock.";
}

void ClockApp::synthesizeClockReadings()
{
    /* Synthesizing hour readings */
    voiceManager_.synthesizeVoiceMessage("<speak>północ. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/hours/", "hour_0");

    for (int hourIndex = 1; hourIndex <= 23; ++hourIndex)
        voiceManager_.synthesizeVoiceMessage("<speak>" + std::to_string(hourIndex) + ":00. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/hours/", "hour_" + std::to_string(hourIndex));
    
    /* Synthesizing minute readings */
    for (int minuteIndex = 1; minuteIndex <= 9; ++minuteIndex)
        voiceManager_.synthesizeVoiceMessage("<speak>0" + std::to_string(minuteIndex) + ". </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/minutes/", "minute_" + std::to_string(minuteIndex));

    for (int minuteIndex = 10; minuteIndex <= 59; ++minuteIndex)
        voiceManager_.synthesizeVoiceMessage("<speak>" + std::to_string(minuteIndex) + ". </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/minutes/", "minute_" + std::to_string(minuteIndex));

    /* Synthesizing weekday readings */
    voiceManager_.synthesizeVoiceMessage("<speak>niedziela. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/weekdays/", "weekday_1");
    voiceManager_.synthesizeVoiceMessage("<speak>poniedziałek. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/weekdays/", "weekday_2");
    voiceManager_.synthesizeVoiceMessage("<speak>wtorek. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/weekdays/", "weekday_3");    
    voiceManager_.synthesizeVoiceMessage("<speak>środa. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/weekdays/", "weekday_4");
    voiceManager_.synthesizeVoiceMessage("<speak>czwartek. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/weekdays/", "weekday_5");
    voiceManager_.synthesizeVoiceMessage("<speak>piątek. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/weekdays/", "weekday_6");
    voiceManager_.synthesizeVoiceMessage("<speak>sobota. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/weekdays/", "weekday_7");

    /* Synthesizing day readings */
    for (int dayIndex = 1; dayIndex <= 31; ++dayIndex)
    {
        if ((dayIndex % 10 == 2 || dayIndex % 10 == 3) && dayIndex != 12 && dayIndex != 13)
            voiceManager_.synthesizeVoiceMessage("<speak>" + std::to_string(dayIndex) + "-i. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/days/", "day_" + std::to_string(dayIndex));
        else if (dayIndex % 10 == 4)
            voiceManager_.synthesizeVoiceMessage("<speak>" + std::to_string(dayIndex) + "-ty. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/days/", "day_" + std::to_string(dayIndex));
        else
            voiceManager_.synthesizeVoiceMessage("<speak>" + std::to_string(dayIndex) + "-y. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/days/", "day_" + std::to_string(dayIndex));   
    }

    /* Synthesizing month readings */
    voiceManager_.synthesizeVoiceMessage("<speak>stycznia. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/months/", "month_1");
    voiceManager_.synthesizeVoiceMessage("<speak>lutego. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/months/", "month_2"); 
    voiceManager_.synthesizeVoiceMessage("<speak>marca. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/months/", "month_3");
    voiceManager_.synthesizeVoiceMessage("<speak>kwietnia. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/months/", "month_4");
    voiceManager_.synthesizeVoiceMessage("<speak>maja. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/months/", "month_5");
    voiceManager_.synthesizeVoiceMessage("<speak>czerwca. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/months/", "month_6");
    voiceManager_.synthesizeVoiceMessage("<speak>lipca. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/months/", "month_7");
    voiceManager_.synthesizeVoiceMessage("<speak>sierpnia. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/months/", "month_8");
    voiceManager_.synthesizeVoiceMessage("<speak>września. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/months/", "month_9");
    voiceManager_.synthesizeVoiceMessage("<speak>października. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/months/", "month_10");
    voiceManager_.synthesizeVoiceMessage("<speak>listopada. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/months/", "month_11");
    voiceManager_.synthesizeVoiceMessage("<speak>grudnia. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/months/", "month_12");

    /* Synthesizing year readings */
    for (int yearIndex = 0; yearIndex <= 100; ++yearIndex)
    {
        if ((2019 + yearIndex) % 10 == 2 || (2019 + yearIndex) % 10 == 3)
            voiceManager_.synthesizeVoiceMessage("<speak>" + std::to_string(2019 + yearIndex) + "-i. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/years/", "year_" + std::to_string(2019 + yearIndex));
        else if ((2019 + yearIndex) % 10 == 4)
            voiceManager_.synthesizeVoiceMessage("<speak>" + std::to_string(2019 + yearIndex) + "-ty. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/years/", "year_" + std::to_string(2019 + yearIndex));
        else
            voiceManager_.synthesizeVoiceMessage("<speak>" + std::to_string(2019 + yearIndex) + "-y. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/years/", "year_" + std::to_string(2019 + yearIndex));   
    }
}


void ClockApp::appLoopFunction()
{
    while (true)
    {   
        inputManager_.update();
        if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_T)) playCurrentTime();
        if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_D)) playCurrentDate();
    }
}

void ClockApp::playCurrentTime()
{
    time_t sec = time(NULL);
    tm result;
    tm * currentTime = localtime_r(&sec, &result);

    std::cout << "hour_" + std::to_string(currentTime->tm_hour) << std::endl;
    std::cout << "minute_" + std::to_string(currentTime->tm_min) << std::endl;

    audioManager_.playMultipleAndGetLastAudioStream(std::vector<AudioTrack>({ 
        voiceManager_.getSynthesizedVoiceAudioTracks().at("hour_" + std::to_string(currentTime->tm_hour)),
        voiceManager_.getSynthesizedVoiceAudioTracks().at("minute_" + std::to_string(currentTime->tm_min))
    }));
}

void ClockApp::playCurrentDate()
{
    time_t sec = time(NULL);
    tm result;
    tm * currentTime = localtime_r(&sec, &result);

    audioManager_.playMultipleAndGetLastAudioStream(std::vector<AudioTrack>({ 
        voiceManager_.getSynthesizedVoiceAudioTracks().at("weekday_" + std::to_string(currentTime->tm_wday + 1)),
        voiceManager_.getSynthesizedVoiceAudioTracks().at("day_" + std::to_string(currentTime->tm_mday)),
        voiceManager_.getSynthesizedVoiceAudioTracks().at("month_" + std::to_string(currentTime->tm_mon + 1)),
        voiceManager_.getSynthesizedVoiceAudioTracks().at("year_" + std::to_string(currentTime->tm_year + 1900))
    }));
}

}