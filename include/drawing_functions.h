#ifndef DRAWINGFUNCS_H
#define DRAWINGFUNCS_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "cleanup.h"
#include "res_path.h"

using namespace std;

/**
 * Loads an image into a texture on the rendering device
 * @param file The image file to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or nullptr if something went wrong.
 */
SDL_Texture *loadTexture(const string &file, SDL_Renderer *ren);

/**
 * Loads an image up as a surface. Generally we want to do this if we want to palette swap
 */
SDL_Surface *loadSurface(const string &file, SDL_Renderer *ren);

/**
 *Copies the palette of 1 surface to another
 */
void surfacePaletteSwap(SDL_Surface *surface, SDL_Surface *palette);

/**
 *Converts a surface to a texture and optionally deletes the surface
 */
SDL_Texture *convertSurfaceToTexture(SDL_Surface *surface, SDL_Renderer *ren, bool cleanSurface = false);

/**
 * Draw an SDL_Texture to an SDL_Renderer at some destination rect
 * taking a clip of the texture if desired
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw to
 * @param dst The destination rectangle to render the texture to
 * @param clip The sub-section of the texture to draw (clipping rect)
 *		default of nullptr draws the entire texture
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
                   SDL_Rect *clip = nullptr);
/**
 * Render the message we want to display to a texture for drawing
 * @param message The message we want to display
 * @param fontFile The font we want to use to render the text
 * @param color The color we want the text to be
 * @param fontSize The size we want the font to be
 * @param renderer The renderer to load the texture in
 * @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
 */

/**
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height and taking a clip of the texture if desired
 * If a clip is passed, the clip's width and height will be used instead of
 *	the texture's
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw to
 * @param x The x coordinate to draw to
 * @param y The y coordinate to draw to
 * @param clip The sub-section of the texture to draw (clipping rect)
 *		default of nullptr draws the entire texture
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
                   SDL_Rect *clip = nullptr);

SDL_Texture *renderText(const string &message, const string &frontFile, SDL_Color color, int frontSize, SDL_Renderer *renderer);

/**
 * Render the message we want to display to a texture for drawing
 * @param message The message we want to display
 * @param font The font we want to use to render the text
 * @param color The color we want the text to be
 * @param fontSize The size we want the font to be
 * @param renderer The renderer to load the texture in
 * @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
 */
SDL_Texture *renderText(const string message, TTF_Font *font, SDL_Color color, SDL_Renderer *renderer);

bool saveScreenshotBMP(string filepath, SDL_Window *window, SDL_Renderer *renderer);

#endif