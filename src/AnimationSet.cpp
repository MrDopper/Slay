// #include "AnimationSet.h"

// AnimationSet::~AnimationSet()
// {
//     cleanup(spriteSheet);
//     if (whiteSpriteSheet != nullptr)
//     {
//         cleanup(whiteSpriteSheet);
//     }
// }

// Animation *AnimationSet::getAnimation(string name)
// {
//     for (list<Animation>::iterator animation = animations.begin(); animation != animations.end(); animation++)
//     {
//         Animation *anim = &(*animation);
//         // If we find a match on name, return animation from the list
//         if (name == anim->name)
//         {
//             return anim;
//         }
//     }
//     return nullptr;
// }
// // loads a fdset file, also takes a list of what types of data we expect to loading.
// // Also, if we're working with an 8bit image and we know in that 8bit image what coloured pixel in the palette
// // we would prefer to be transparent, then we can say yes to 'colorKeying'(make a colour in the palette actually equal full transparent)
// // and then just use the index of that transparentPixel (e.g, if its the third colour in the palette, then put in 2 as index starts at 0)
// // if you need an alternative white version of the sprite sheet, then make this last option true (maybe move this out of this class? not sure)
// void AnimationSet::loadAnimationSet(string fileName, list<DataGroupType> &groupTypes, bool setColourKey, int transparentPixelIndex, bool createWhiteTexture)
// {
//     ifstream file;
//     string resPath = getResourcePath();
//     file.open(resPath + fileName);
//     if (file.good())
//     {
//         getline(file, imageName);
//         if (setColourKey)
//         {
//             SDL_Surface *spriteSurface = loadSurface(resPath + imageName, Globals::renderer);

//             // for transparency, we will grab the [transparentPixelIndex] from the surface we just made
//             SDL_Color *transparentPixel = &spriteSurface->format->palette->colors[transparentPixelIndex];
//             SDL_SetColorKey(spriteSurface, 1, SDL_MapRGB(spriteSurface->format, transparentPixel->r, transparentPixel->g, transparentPixel->b));

//             spriteSheet = convertSurfaceToTexture(spriteSurface, Globals::renderer, false);

//             if (createWhiteTexture)
//             {
//                 SDL_Surface *whiteSurface = loadSurface(resPath + "allwhite.png", Globals::renderer);
//                 surfacePaletteSwap(spriteSurface, whiteSurface);
//                 SDL_SetColorKey(spriteSurface, 1, SDL_MapRGB(spriteSurface->format, transparentPixel->r, transparentPixel->g, transparentPixel->b));
//                 whiteSpriteSheet = convertSurfaceToTexture(spriteSurface, Globals::renderer, false); // create the texture whilst destroying the surface

//                 cleanup(whiteSurface);
//             }
//             else
//             {
//                 whiteSpriteSheet = NULL;
//             }

//             cleanup(spriteSurface);
//         }
//         else
//         {
//             spriteSheet = loadTexture(resPath + imageName, Globals::renderer);
//         }

//         if (spriteSheet == nullptr)
//         {
//             std::cerr << "Error: Failed to load sprite sheet: " << imageName << std::endl;
//             return;
//         }

//         string buffer;
//         getline(file, buffer);
//         stringstream ss;
//         buffer = Globals::clipOffDataHeader(buffer);

//         // Load animations here...
//         // Example:
//         int numAnimations;
//         ss >> numAnimations;
//         std::cout << "Number of animations to load: " << numAnimations << std::endl;

//         // After loading animations, print the count
//         std::cout << "Animations loaded: " << animations.size() << std::endl;
//     }
//     else
//     {
//         std::cerr << "Error: Failed to open animation file: " << fileName << std::endl;
//     }

//     string buffer;
//     getline(file, buffer);
//     stringstream ss;
//     buffer = Globals::clipOffDataHeader(buffer);
//     ss << buffer;
//     int numberOfAnimations;
//     ss >> numberOfAnimations;

//     for (int i = 0; i < numberOfAnimations; i++)
//     {
//         Animation newAnimation;
//         newAnimation.loadAnimation(file, groupTypes);
//         animations.push_back(newAnimation);
//     }
//     file.close();
// }

#include "AnimationSet.h"

AnimationSet::~AnimationSet()
{
    if (spriteSheet)
    {
        SDL_DestroyTexture(spriteSheet);
    }
    if (whiteSpriteSheet)
    {
        SDL_DestroyTexture(whiteSpriteSheet);
    }
}

Animation *AnimationSet::getAnimation(const std::string &name)
{
    for (auto &animation : animations)
    {
        if (animation.name == name)
        {
            return &animation;
        }
    }
    return nullptr;
}

void AnimationSet::loadAnimationSet(const std::string &fileName, std::list<DataGroupType> &groupTypes, bool setColourKey, int transparentPixelIndex, bool createWhiteTexture)
{
    std::ifstream file;
    std::string resPath = getResourcePath();
    file.open(resPath + fileName);

    if (!file.is_open())
    {
        std::cerr << "Error: Failed to open animation file: " << resPath + fileName << std::endl;
        return;
    }

    std::getline(file, imageName);

    if (setColourKey)
    {
        SDL_Surface *spriteSurface = IMG_Load((resPath + imageName).c_str());
        if (!spriteSurface)
        {
            std::cerr << "Error: Failed to load sprite surface from " << imageName << " - " << IMG_GetError() << std::endl;
            return;
        }

        SDL_Color *transparentPixel = &((SDL_Color *)spriteSurface->pixels)[transparentPixelIndex];
        SDL_SetColorKey(spriteSurface, SDL_TRUE, SDL_MapRGB(spriteSurface->format, transparentPixel->r, transparentPixel->g, transparentPixel->b));
        spriteSheet = SDL_CreateTextureFromSurface(Globals::renderer, spriteSurface);
        SDL_FreeSurface(spriteSurface);
    }
    else
    {
        spriteSheet = IMG_LoadTexture(Globals::renderer, (resPath + imageName).c_str());
    }

    if (!spriteSheet)
    {
        std::cerr << "Error: Failed to load sprite sheet: " << imageName << " - " << IMG_GetError() << std::endl;
        return;
    }

    std::string buffer;
    std::getline(file, buffer);
    buffer = Globals::clipOffDataHeader(buffer);

    std::stringstream ss(buffer);
    int numAnimations;
    ss >> numAnimations;

    if (ss.fail() || numAnimations <= 0)
    {
        std::cerr << "Error: Invalid number of animations in " << fileName << std::endl;
        return;
    }

    for (int i = 0; i < numAnimations; ++i)
    {
        Animation anim;

        // Parse animation name
        if (!(file >> anim.name))
        {
            std::cerr << "Error: Failed to read animation name in " << fileName << std::endl;
            return;
        }

        // Parse the number of frames
        int numFrames;
        if (!(file >> numFrames) || numFrames <= 0)
        {
            std::cerr << "Error: Invalid number of frames for animation " << anim.name << std::endl;
            return;
        }

        for (int j = 0; j < numFrames; ++j)
        {
            Frame frame;

            // Read frame clip coordinates and size
            if (!(file >> frame.clip.x >> frame.clip.y >> frame.clip.w >> frame.clip.h))
            {
                std::cerr << "Error: Failed to read frame clip for animation " << anim.name << std::endl;
                return;
            }

            // Read frame offset
            if (!(file >> frame.offset.x >> frame.offset.y))
            {
                std::cerr << "Error: Failed to read frame offset for animation " << anim.name << std::endl;
                return;
            }

            // Read frame duration
            if (!(file >> frame.duration))
            {
                std::cerr << "Error: Failed to read frame duration for animation " << anim.name << std::endl;
                return;
            }

            anim.frames.push_back(frame);
        }

        animations.push_back(anim);
    }

    std::cout << "Animations loaded: " << animations.size() << " from " << fileName << std::endl;
    file.close();
}
