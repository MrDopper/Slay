#include "keyboardInput.h"

KeyboardInput::KeyboardInput()
{
    UP = SDL_SCANCODE_UP;
    DOWN = SDL_SCANCODE_DOWN;
    LEFT = SDL_SCANCODE_LEFT;
    RIGHT = SDL_SCANCODE_RIGHT;
    SLASH = SDL_SCANCODE_Z;
    DASH = SDL_SCANCODE_X;
}
void KeyboardInput::update(SDL_Event *e)
{
    if (e->type == SDL_KEYDOWN)
    {
        if (e->key.keysym.scancode == DASH)
        {
            hero->dash();
        }
        if (e->key.keysym.scancode == SLASH)
        {
            hero->slash();
        }
    }
    // button hold

    const Uint8 *keyStates = SDL_GetKeyboardState(NULL);
    // Character stops when no button is being press
    if ((hero->state != Hero::HERO_STATE_MOVE && hero->state != Hero::HERO_STATE_IDLE) || (!keyStates[UP] && !keyStates[DOWN] && !keyStates[LEFT] && !keyStates[RIGHT]))
    {
        hero->moving = false;
    }
    else
    {
        // ups
        if (keyStates[UP])
        {
            // upright
            if (keyStates[RIGHT])
                hero->move(270 + 45);
            else if (keyStates[LEFT])
                hero->move(270 - 45);
            else
                hero->move(270);
        }
        // downs
        if (keyStates[DOWN])
        {
            // downright
            if (keyStates[RIGHT])
                hero->move(90 - 45);
            else if (keyStates[LEFT])
                hero->move(90 + 45);
            else
                hero->move(90);
        }
        // left
        if (!keyStates[UP] && !keyStates[DOWN] && !keyStates[RIGHT] && keyStates[LEFT])
            hero->move(180);
        // right
        if (!keyStates[UP] && !keyStates[DOWN] && keyStates[RIGHT] && !keyStates[LEFT])
            hero->move(0);
    }
}