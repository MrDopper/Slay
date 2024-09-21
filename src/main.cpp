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
    SDL_Window *window = SDL_CreateWindow("Cybot_game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Globals::ScreenWidth * Globals::ScreenScale, Globals::ScreenHeight * Globals::ScreenScale, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

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
    cleanup(Globals::renderer);
    cleanup(window);

    SafeQuit();
    cout << "Application exited cleanly!" << endl;

    return 0;
}
