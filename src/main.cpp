
// #include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
// #include <SDL2/SDL_ttf.h>
// #include <SDL2/SDL_mixer.h>
// #include <iostream>
// #include "cleanup.h"
// #include "res_path.h"
// #include "drawing_functions.h"
// #include "Globals.h"
// #include "hero.h"
// #include "AnimationSet.h"

// using std::cout, std::endl, std::string;

// /**
//  * Shutdown SDL_mixer, SDL_ttf, SDL_image, and SDL
//  */

// void SafeQuit()
// {
//     Mix_CloseAudio();
//     TTF_Quit();
//     IMG_Quit();
//     SDL_Quit();
// }

// int main(int argc, char *argv[])
// {
//     // Initialize SDL
//     cout << "Initializing SDL..." << endl;
//     if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
//     {
//         cout << "SDL_Init Error: " << SDL_GetError() << endl;
//         return 1;
//     }
//     cout << "SDL initialized successfully!" << endl;

//     // Create Window
//     cout << "Creating window..." << endl;
//     SDL_Window *window = SDL_CreateWindow("Cybot_game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Globals::ScreenWidth * Globals::ScreenScale, Globals::ScreenHeight * Globals::ScreenScale, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

//     if (window == nullptr)
//     {
//         cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
//         SDL_Quit();
//         return 1;
//     }
//     cout << "Window created successfully!" << endl;

//     // Create Renderer
//     cout << "Creating renderer..." << endl;
//     Globals::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//     if (Globals::renderer == nullptr)
//     {
//         cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
//         SDL_DestroyWindow(window);
//         SDL_Quit();
//         return 1;
//     }
//     SDL_RenderSetLogicalSize(Globals::renderer, Globals::ScreenWidth, Globals::ScreenHeight);
//     cout << "Renderer created successfully!" << endl;

//     // Initialize SDL_image for PNG support
//     cout << "Initializing SDL_image..." << endl;
//     if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
//     {
//         cout << "IMG_Init Error: " << IMG_GetError() << endl;
//         SDL_DestroyRenderer(Globals::renderer);
//         SDL_DestroyWindow(window);
//         SDL_Quit();
//         return 1;
//     }
//     cout << "SDL_image initialized successfully!" << endl;

//     // Initialize SDL_ttf for font support
//     cout << "Initializing SDL_ttf..." << endl;
//     if (TTF_Init() != 0)
//     {
//         cout << "TTF_Init Error: " << TTF_GetError() << endl;
//         IMG_Quit();
//         SDL_DestroyRenderer(Globals::renderer);
//         SDL_DestroyWindow(window);
//         SDL_Quit();
//         return 1;
//     }
//     cout << "SDL_ttf initialized successfully!" << endl;

//     // Initialize SDL_mixer for audio support
//     cout << "Initializing SDL_mixer..." << endl;
//     if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
//     {
//         cout << "Mix_OpenAudio Error: " << Mix_GetError() << endl;
//         TTF_Quit();
//         IMG_Quit();
//         SDL_DestroyRenderer(Globals::renderer);
//         SDL_DestroyWindow(window);
//         SDL_Quit();
//         return 1;
//     }
//     cout << "SDL_mixer initialized successfully!" << endl;

//     // Load a texture to draw
//     cout << "Loading texture..." << endl;

//     string resPath = getResourcePath(); // Assuming getResourcePath() returns the correct path
//     SDL_Texture *texture = loadTexture(resPath + "map.png", Globals::renderer);
//     if (!texture)
//     {
//         cout << "Failed to load texture: " << SDL_GetError() << endl;
//         Mix_CloseAudio();
//         TTF_Quit();
//         IMG_Quit();
//         SDL_DestroyRenderer(Globals::renderer);
//         SDL_DestroyWindow(window);
//         SDL_Quit();
//         return 1;
//     }
//     cout << "Texture loaded successfully!" << endl;

//     // Run game loop for 5000 ticks
//     cout << "Entering main loop..." << endl;
//     string fullPath = getResourcePath() + "hero.fdset";
//     std::ifstream file(fullPath);
//     if (!file)
//     {
//         std::cerr << "Failed to open file: " << fullPath << std::endl;
//     }
//     else
//     {
//         std::cout << "Successfully opened file: " << fullPath << std::endl;
//     }

//     SDL_Event e;
//     bool quit = false;

//     while (!quit)
//     {
//         while (SDL_PollEvent(&e))
//         {
//             if (e.type == SDL_QUIT)
//             {
//                 quit = true;
//             }
//             // if (e.type == SDL_KEYDOWN)
//             // {
//             //     switch (e.key.keysym.scancode)
//             //     {
//             //     case SDL_SCANCODE_ESCAPE:
//             //         quit = true;
//             //         break;
//             //     case SDL_SCANCODE_SPACE:
//             //         break;
//             //     }
//             // }
//         }

//         // Clear the screen
//         SDL_RenderClear(Globals::renderer);
//         // Draw the texture
//         renderTexture(texture, Globals::renderer, 0, 0);
//         // Present the rendered image
//         SDL_RenderPresent(Globals::renderer);
//     }

//     // Cleanup resources
//     cleanup(texture);

//     // Game game;
//     // game.update();

//     cleanup(Globals::renderer);
//     cleanup(window);

//     SafeQuit();
//     cout << "Application exited cleanly!" << endl;

//     return 0;
// }

// #include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
// #include <iostream>
// #include <vector>
// #include <list>
// #include "Globals.h"
// #include "wall.h"
// #include "AnimationSet.h"
// #include "dataGroupType.h"
// #include "cleanup.h"
// #include "res_path.h"

// // Assuming HitBox structure exists in your codebase
// struct HitBox
// {
//     int x, y, width, height;
// };

// void loadWallData(const std::string &filePath, AnimationSet &wallAnimSet, std::vector<HitBox> &hitBoxes, int &damageValue)
// {
//     std::ifstream file(filePath);
//     if (!file.is_open())
//     {
//         std::cerr << "Error: Failed to open animation file: " << filePath << std::endl;
//         return;
//     }

//     std::string textureFile;
//     std::string animationName;
//     int animations, frames;

//     // Start reading the wall.txt file
//     if (!(file >> textureFile))
//     {
//         std::cerr << "Error: Failed to read texture file name from " << filePath << std::endl;
//         return;
//     }
//     if (!(file >> animations >> animationName))
//     {
//         std::cerr << "Error: Failed to read animation count or name from " << filePath << std::endl;
//         return;
//     }
//     if (!(file >> frames))
//     {
//         std::cerr << "Error: Failed to read number of frames from " << filePath << std::endl;
//         return;
//     }

//     int clipX, clipY, clipW, clipH;
//     int offsetX, offsetY;
//     float duration;
//     int collisionX, collisionY, collisionW, collisionH;

//     // Read the rest of the animation details
//     if (!(file >> clipX >> clipY >> clipW >> clipH))
//     {
//         std::cerr << "Error: Failed to read clip dimensions from " << filePath << std::endl;
//         return;
//     }
//     if (!(file >> offsetX >> offsetY))
//     {
//         std::cerr << "Error: Failed to read offsets from " << filePath << std::endl;
//         return;
//     }
//     if (!(file >> duration))
//     {
//         std::cerr << "Error: Failed to read frame duration from " << filePath << std::endl;
//         return;
//     }
//     if (!(file >> collisionX >> collisionY >> collisionW >> collisionH))
//     {
//         std::cerr << "Error: Failed to read collision box dimensions from " << filePath << std::endl;
//         return;
//     }

//     // Load the sprite sheet (wall.png)
//     wallAnimSet.spriteSheet = IMG_LoadTexture(Globals::renderer, (getResourcePath() + textureFile).c_str());
//     if (wallAnimSet.spriteSheet == nullptr)
//     {
//         std::cerr << "Failed to load sprite sheet: " << textureFile << " Error: " << IMG_GetError() << std::endl;
//         return;
//     }

//     // Set up the hitbox from the collision box in the wall.txt
//     HitBox hitBox = {collisionX, collisionY, collisionW, collisionH};
//     hitBoxes.push_back(hitBox);

//     // Set up groupTypes for loading animations
//     std::list<DataGroupType> groupTypes;

//     DataGroupType colBoxType;
//     colBoxType.groupName = "collisionBox";
//     colBoxType.dataType = DataGroupType::DATATYPE_BOX;

//     DataGroupType hitBoxType;
//     hitBoxType.groupName = "hitBox";
//     hitBoxType.dataType = DataGroupType::DATATYPE_BOX;

//     DataGroupType dmgType;
//     dmgType.groupName = "damage";
//     dmgType.dataType = DataGroupType::DATATYPE_NUMBER;

//     groupTypes.push_back(colBoxType);
//     groupTypes.push_back(hitBoxType);
//     groupTypes.push_back(dmgType);

//     wallAnimSet.loadAnimationSet(filePath, groupTypes, true, 0, false);

//     std::cout << "Wall data loaded successfully from " << filePath << std::endl;
// }

// int main(int argc, char *argv[])
// {
//     if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
//     {
//         std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
//         return 1;
//     }

//     SDL_Window *window = SDL_CreateWindow("Wall Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Globals::ScreenWidth * Globals::ScreenScale, Globals::ScreenHeight * Globals::ScreenScale, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
//     if (!window)
//     {
//         std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
//         SDL_Quit();
//         return 1;
//     }

//     Globals::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//     if (!Globals::renderer)
//     {
//         std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
//         SDL_DestroyWindow(window);
//         SDL_Quit();
//         return 1;
//     }

//     std::vector<HitBox> hitBoxes;
//     int damageValue = 0;
//     std::string resPath = getResourcePath();
//     AnimationSet wallAnimSet;

//     loadWallData(resPath + "wall.txt", wallAnimSet, hitBoxes, damageValue);

//     // Create a Wall object using the loaded animation set
//     Wall wall(&wallAnimSet);
//     wall.x = 100; // Set X position
//     wall.y = 100; // Set Y position

//     bool quit = false;
//     SDL_Event event;
//     while (!quit)
//     {
//         while (SDL_PollEvent(&event))
//         {
//             if (event.type == SDL_QUIT)
//             {
//                 quit = true;
//             }
//         }

//         SDL_SetRenderDrawColor(Globals::renderer, 0, 0, 0, 255);
//         SDL_RenderClear(Globals::renderer);

//         // Draw the wall
//         wall.draw();

//         SDL_RenderPresent(Globals::renderer);
//     }

//     SDL_DestroyRenderer(Globals::renderer);
//     SDL_DestroyWindow(window);
//     SDL_Quit();

//     return 0;
// }

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include "cleanup.h"
#include "res_path.h"
#include "drawing_functions.h"
#include "Globals.h"

using std::cout;
using std::endl;
using std::string;

/**
 * Shutdown SDL_mixer, SDL_ttf, SDL_image, and SDL
 */
void SafeQuit()
{
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char *argv[])
{
    // Initialize SDL
    cout << "Initializing SDL..." << endl;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        cout << "SDL_Init Error: " << SDL_GetError() << endl;
        return 1;
    }
    cout << "SDL initialized successfully!" << endl;

    // Create Window
    cout << "Creating window..." << endl;
    SDL_Window *window = SDL_CreateWindow("Slay", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Globals::ScreenWidth * Globals::ScreenScale, Globals::ScreenHeight * Globals::ScreenScale, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (window == nullptr)
    {
        cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }
    cout << "Window created successfully!" << endl;

    // Create Renderer
    cout << "Creating renderer..." << endl;
    Globals::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (Globals::renderer == nullptr)
    {
        cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_RenderSetLogicalSize(Globals::renderer, Globals::ScreenWidth, Globals::ScreenHeight);
    cout << "Renderer created successfully!" << endl;

    // Initialize SDL_image for PNG support
    cout << "Initializing SDL_image..." << endl;
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        cout << "IMG_Init Error: " << IMG_GetError() << endl;
        SDL_DestroyRenderer(Globals::renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    cout << "SDL_image initialized successfully!" << endl;

    // Initialize SDL_ttf for font support
    cout << "Initializing SDL_ttf..." << endl;
    if (TTF_Init() != 0)
    {
        cout << "TTF_Init Error: " << TTF_GetError() << endl;
        IMG_Quit();
        SDL_DestroyRenderer(Globals::renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    cout << "SDL_ttf initialized successfully!" << endl;

    // Initialize SDL_mixer for audio support
    cout << "Initializing SDL_mixer..." << endl;
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        cout << "Mix_OpenAudio Error: " << Mix_GetError() << endl;
        TTF_Quit();
        IMG_Quit();
        SDL_DestroyRenderer(Globals::renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    cout << "SDL_mixer initialized successfully!" << endl;

    // Load a texture to draw
    cout << "Loading texture..." << endl;
    string resPath = getResourcePath(); // Assuming getResourcePath() returns the correct path
    SDL_Texture *texture = loadTexture(resPath + "map.png", Globals::renderer);
    if (!texture)
    {
        cout << "Failed to load texture: " << SDL_GetError() << endl;
        Mix_CloseAudio();
        TTF_Quit();
        IMG_Quit();
        SDL_DestroyRenderer(Globals::renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    cout << "Texture loaded successfully!" << endl;

    // Run game loop for 5000 ticks
    cout << "Entering main loop..." << endl;
    SDL_Event e;
    bool quit = false;
    while (!quit && SDL_GetTicks() < 5000)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // Clear the screen
        SDL_RenderClear(Globals::renderer);
        // Draw the texture
        renderTexture(texture, Globals::renderer, 0, 0);
        // Present the rendered image
        SDL_RenderPresent(Globals::renderer);
    }

    // Cleanup resources
    cleanup(texture);
    // Game game;
    // game.update();

    cleanup(Globals::renderer);
    cleanup(window);

    SafeQuit();
    cout << "Application exited cleanly!" << endl;

    return 0;
}
