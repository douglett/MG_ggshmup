#pragma once
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <SDL/SDL.h>

extern SDL_Surface *buf;
int  mainloop();
void paint1();

void scalex(SDL_Surface* sf, int sx);
SDL_Surface* loadbmp(const std::string& fname);

