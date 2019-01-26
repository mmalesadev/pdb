#include "PDBAudiobook.h"
#include <iostream>

void PDBAudiobook::init()
{
    std::cout << "Initialized PDBAudiobook.\n";
}

void PDBAudiobook::appLoopFunction()
{
   std::cout << "Starting PDBAudiobook loop function.\n";
   while(true)
   {

   }
   std::cout << "Ending PDBAudiobook loop function.\n";
}

void playMessage(const std::string & message)
{
    std::string milenaMessageCall = "milena_say " + message;
    system(milenaMessageCall.c_str());
}