#ifndef CLEANUP_H
#define CLEANUP_H

#include <utility>
#include <SDL2/SDL.h>

void cleanup(SDL_Window *win);

void cleanup(SDL_Renderer *ren);

void cleanup(SDL_Texture *tex);

void cleanup(SDL_Surface *surf);

#endif