#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL2/SDL.h>
#include <cstring>
#include <iostream>
#include <randomNumber.h>
using namespace std;

class Globals
{
public:
    static const float PI;
    static bool debugging;
    static int ScreenWidth, ScreenHeight, ScreenScale;
    static SDL_Renderer *renderer;
    // clip off header. e.g "clip: 50 114 44 49" turns into "50 114 44 49"
    static string clipOffDataHeader(string data);
};

#endif
