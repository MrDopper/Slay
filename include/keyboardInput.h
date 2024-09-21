#ifndef KEYBOARDINPUT_H
#define KEYBOARDINPUT_H

#include "hero.h"

class KeyboardInput
{
public:
    // which character I am current choose
    Hero *hero;
    SDL_Scancode UP, DOWN, LEFT, RIGHT;
    SDL_Scancode SLASH, DASH;

    KeyboardInput();
    void update(SDL_Event *e);
};

#endif