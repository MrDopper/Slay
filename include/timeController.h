#ifndef TIMECONTROLLER_H
#define TIMECONTROLLER_H

#include <SDL2/SDL.h>
#include <iostream>

class TimeController
{
public:
    static const int PLAY_STATE, PAUSE_STATE;
    int timeState;
    // Last time we checked how many tick we were up
    Uint32 lastUpdate;
    // Delta time, in second(1 = 1 second, and 0.5 is a half second) Time since the last time frame was render onto screen.
    float deltaTime;

    TimeController();
    // Update lastUpdate and deltaTime
    void updatetime();
    void pause();
    void resume();
    void reset();

    static TimeController timeController;
};

#endif