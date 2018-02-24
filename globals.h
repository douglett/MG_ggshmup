#pragma once
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <SDL/SDL.h>

extern SDL_Surface *buf;
int  mainloop();
void walk(int dir);
void paint1();
void flip3x();

void scalex(SDL_Surface* sf, int sx);
SDL_Surface* loadbmp(const std::string& fname);

namespace map {
	extern int width, height;
	extern std::vector<std::vector<int>> tmap;
	int loadmap(const std::string& fname);
}
