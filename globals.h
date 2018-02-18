#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include <SDL/SDL.h>

extern SDL_Surface *buf;
int  mainloop();
void paint1();

void scale2x(SDL_Surface* sf);
void scale3x(SDL_Surface* sf);
void scalex(SDL_Surface* sf, int sx);
