#ifndef FRAME_H
#define FRAME_H
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <SDL2/SDL.h>
#include "drawing_functions.h"
#include "Globals.h"
#include "groupBuilder.h"

using namespace std;
class Frame
{
public:
    // Or frame index
    int frameNumber;
    // It the region on the spritesheet where this frame is
    SDL_Rect clip;
    // How long the frame will run for
    float duration;
    // Pivot point to help align frames in an animation.
    SDL_Point offSet;
    // holds groups of addtional data for the frame
    list<Group *> frameData;
    void draw(SDL_Texture *spriteSheet, float x, float y);
    void loadFrame(ifstream &file, list<DataGroupType> &groupTypes);
};

#endif