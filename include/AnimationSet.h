#ifndef ANIMATIONSET_H
#define ANIMATIONSET_H
#include "animation.h"

class AnimationSet
{
public:
    string imageName;
    // Hold the animation spriteSheet for our animations
    SDL_Texture *spriteSheet;
    // I use this spriteSheet to show damage
    SDL_Texture *whiteSpriteSheet = nullptr;
    list<Animation> animations;
    //properly clean up any dynamic memory e.g textures
    ~AnimationSet();

    Animation* getAnimation(string name);

    void loadAnimationSet(string fileName, list<DataGroupType> &groupTypes, bool  setColourKey =  false, int transparentPixelIndex = 0, bool createWhiteTexture = false);
};

#endif