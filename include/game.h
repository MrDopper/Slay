#ifndef GAME_H
#define GAME_H

#include "hero.h"
#include "wall.h"
#include "drawing_functions.h"
#include "Globals.h"
#include "keyboardInput.h"

class Game
{
public:
    AnimationSet *heroAnimate;
    AnimationSet *wallAnimate;

    SDL_Texture *backgroundImage;
    Hero *hero;
    KeyboardInput heroInput;

    list<Entity *> walls;

    Game();
    ~Game();
    void update();
    void draw();
};
#endif