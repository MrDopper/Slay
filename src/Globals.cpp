#include "Globals.h"

const float Globals::PI = 3.14159;

bool Globals::debugging = true;

int Globals::ScreenWidth = 640, Globals::ScreenHeight = 352, Globals::ScreenScale = 2;
SDL_Renderer *Globals::renderer = nullptr;

string Globals::clipOffDataHeader(string data)
{
    // Return where we find it in the string, otherwise, return -1
    int pos = data.find(":", 0);
    if (pos != -1)
    {
        data = data.substr(pos + 1, data.length() - pos + 2);
    }
    return data; 
}