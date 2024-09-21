#include "timeController.h"

// Refenence values
const int TimeController::PLAY_STATE = 0, TimeController::PAUSE_STATE = 1;

TimeController::TimeController()
{
    deltaTime = 0;
    lastUpdate = 0;
    timeState = PLAY_STATE;
}
void TimeController::updatetime()
{
    if (timeState == PAUSE_STATE)
    {
        deltaTime = 0;
    }
    else
    {
        Uint32 timeDiff = SDL_GetTicks() - lastUpdate;
        // Move these miliseconds into decimal place area e.g 104 ticks = 0.104 seconds
        deltaTime = timeDiff / 1000.0;
    }
    lastUpdate = SDL_GetTicks();
}
void TimeController::pause()
{
    timeState = PAUSE_STATE;
}
void TimeController::resume()
{
    timeState = PLAY_STATE;
}
void TimeController::reset()
{
    deltaTime = 0;
    lastUpdate = SDL_GetTicks();
}

// Build a global variable timeConctroller object for use anywhere
TimeController TimeController::timeController;