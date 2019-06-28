#include "ClockApp.h"
#include <boost/log/trivial.hpp>
#include <ctime>



namespace Pdb
{

ClockApp::ClockApp(VoiceManager& voiceManager)
    : Pdb::App(voiceManager), currentAudioTask_(nullptr)
{
}

void ClockApp::init()
{
    this->synthesizeClockReadings();
    BOOST_LOG_TRIVIAL(info) << "Initialized Clock.";
}

void ClockApp::synthesizeClockReadings()
{
    /* Synthesizing time readings */
    auto timeSynthesizingFunction = [&](int hourFrom, int hourTo)
    {
        if (hourFrom > hourTo)
            return;

        for (int hour = hourFrom; hour <= hourTo; ++hour)
        {
            if (hour == 0)
            {
                voiceManager_.synthesizeVoiceMessage("<speak>północ. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/time_readings", "time_0_0");
                for (int minute = 1; minute < 60; ++minute)
                    voiceManager_.synthesizeVoiceMessage("<speak>" + std::to_string(minute) + " po północy. </speak>", "../data/synthesized_sounds/apps/clock/readings/pl/time_readings", "time_0_" + std::to_string(minute));
            }
            else
            {
                for (int minute = 0; minute < 60; ++minute)
                {
                    if (minute < 10)
                        voiceManager_.synthesizeVoiceMessage("<speak>" + std::to_string(hour) + ":0" + std::to_string(minute) + "</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/time_readings", "time_" + std::to_string(hour) + "_" + std::to_string(minute));
                    else
                        voiceManager_.synthesizeVoiceMessage("<speak>" + std::to_string(hour) + ":" + std::to_string(minute) + "</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/time_readings", "time_" + std::to_string(hour) + "_" + std::to_string(minute));
                }
            }
        }
    };

    timeSynthesizingFunction(0, 23);

    /* Synthesizing weekday readings */
    voiceManager_.synthesizeVoiceMessage("<speak>niedziela.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/weekdays", "weekday_1");
    voiceManager_.synthesizeVoiceMessage("<speak>poniedziałek.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/weekdays", "weekday_2");
    voiceManager_.synthesizeVoiceMessage("<speak>wtorek.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/weekdays", "weekday_3");    
    voiceManager_.synthesizeVoiceMessage("<speak>środa.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/weekdays", "weekday_4");
    voiceManager_.synthesizeVoiceMessage("<speak>czwartek.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/weekdays", "weekday_5");
    voiceManager_.synthesizeVoiceMessage("<speak>piątek.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/weekdays", "weekday_6");
    voiceManager_.synthesizeVoiceMessage("<speak>sobota.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/weekdays", "weekday_7");

    /* Synthesizing day with month readings */

    std::vector<std::string> dayMonthReadingStrings;

    for (int dayIndex = 1; dayIndex <= 31; ++dayIndex)
    {
        std::string dayReadingStr;
        if ((dayIndex % 10 == 2 || dayIndex % 10 == 3) && dayIndex != 12 && dayIndex != 13)
            dayReadingStr = "<speak>" + std::to_string(dayIndex) + "-i ";
        else if (dayIndex % 10 == 4)
            dayReadingStr = "<speak>" + std::to_string(dayIndex) + "-ty ";
        else
            dayReadingStr = "<speak>" + std::to_string(dayIndex) + "-y ";

        voiceManager_.synthesizeVoiceMessage(dayReadingStr + "stycznia.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/days_with_months", "day_" + std::to_string(dayIndex) + "_month_1");
        voiceManager_.synthesizeVoiceMessage(dayReadingStr + "lutego.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/days_with_months", "day_" + std::to_string(dayIndex) + "_month_2"); 
        voiceManager_.synthesizeVoiceMessage(dayReadingStr + "marca.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/days_with_months", "day_" + std::to_string(dayIndex) + "_month_3");
        voiceManager_.synthesizeVoiceMessage(dayReadingStr + "kwietnia.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/days_with_months", "day_" + std::to_string(dayIndex) + "_month_4");
        voiceManager_.synthesizeVoiceMessage(dayReadingStr + "maja.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/days_with_months", "day_" + std::to_string(dayIndex) + "_month_5");
        voiceManager_.synthesizeVoiceMessage(dayReadingStr + "czerwca.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/days_with_months", "day_" + std::to_string(dayIndex) + "_month_6");
        voiceManager_.synthesizeVoiceMessage(dayReadingStr + "lipca.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/days_with_months", "day_" + std::to_string(dayIndex) + "_month_7");
        voiceManager_.synthesizeVoiceMessage(dayReadingStr + "sierpnia.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/days_with_months", "day_" + std::to_string(dayIndex) + "_month_8");
        voiceManager_.synthesizeVoiceMessage(dayReadingStr + "września.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/days_with_months", "day_" + std::to_string(dayIndex) + "_month_9");
        voiceManager_.synthesizeVoiceMessage(dayReadingStr + "października.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/days_with_months", "day_" + std::to_string(dayIndex) + "_month_10");
        voiceManager_.synthesizeVoiceMessage(dayReadingStr + "listopada.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/days_with_months", "day_" + std::to_string(dayIndex) + "_month_11");
        voiceManager_.synthesizeVoiceMessage(dayReadingStr + "grudnia.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/days_with_months", "day_" + std::to_string(dayIndex) + "_month_12");
    }

    /* Synthesizing year readings */
    for (int yearIndex = 0; yearIndex <= 100; ++yearIndex)
    {
        if ((2019 + yearIndex) % 10 == 2 || (2019 + yearIndex) % 10 == 3)
            voiceManager_.synthesizeVoiceMessage("<speak>" + std::to_string(2019 + yearIndex) + "-i.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/years", "year_" + std::to_string(2019 + yearIndex));
        else if ((2019 + yearIndex) % 10 == 4)
            voiceManager_.synthesizeVoiceMessage("<speak>" + std::to_string(2019 + yearIndex) + "-ty.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/years", "year_" + std::to_string(2019 + yearIndex));
        else
            voiceManager_.synthesizeVoiceMessage("<speak>" + std::to_string(2019 + yearIndex) + "-y.</speak>", "../data/synthesized_sounds/apps/clock/readings/pl/date_readings/years", "year_" + std::to_string(2019 + yearIndex));   
    }
}

void ClockApp::appLoopFunction()
{
    while (true)
    {   
        inputManager_.update();

        if (Config::getInstance().inputMode == "debug")
        {
            if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_UP)) audioManager_.increaseMasterVolume();
            if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_DOWN)) audioManager_.decreaseMasterVolume();
            if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_R)) playCurrentDate();
            if (inputManager_.isButtonPressed(InputManager::Button::BUTTON_T)) playCurrentTime();
        }
        else if (Config::getInstance().inputMode == "prod")
        {
            if (inputManager_.isButtonPressed(InputManager::Button::KeyKpAdd)) audioManager_.increaseMasterVolume();
            if (inputManager_.isButtonPressed(InputManager::Button::KeyKpSubtract)) audioManager_.decreaseMasterVolume();
            if (inputManager_.isButtonPressed(InputManager::Button::KeyKpMultiply)) playCurrentDate();
            if (inputManager_.isButtonPressed(InputManager::Button::KeyKpDivide)) playCurrentTime();
        }
    }
}

void ClockApp::playCurrentTime()
{
    time_t sec = time(NULL);
    tm result;
    tm * currentTime = localtime_r(&sec, &result);
    BOOST_LOG_TRIVIAL(info) << "time_" + std::to_string(currentTime->tm_hour) + "_" + std::to_string(currentTime->tm_min);
    if (currentAudioTask_)
        currentAudioTask_->stop();

    currentAudioTask_ = audioManager_.play({
        voiceManager_.getSynthesizedVoiceAudioTracks().at("time_" + std::to_string(currentTime->tm_hour) + "_" + std::to_string(currentTime->tm_min))
    });
}

void ClockApp::playCurrentDate()
{
    time_t sec = time(NULL);
    tm result;
    tm * currentTime = localtime_r(&sec, &result);
    if (currentAudioTask_)
        currentAudioTask_->stop();

    currentAudioTask_ = audioManager_.play({ voiceManager_.getSynthesizedVoiceAudioTracks().at("weekday_" + std::to_string(currentTime->tm_wday + 1)),
        voiceManager_.getSynthesizedVoiceAudioTracks().at("day_" + std::to_string(currentTime->tm_mday) + "_month_" + std::to_string(currentTime->tm_mon + 1)),
        voiceManager_.getSynthesizedVoiceAudioTracks().at("year_" + std::to_string(currentTime->tm_year + 1900)) });
}

}