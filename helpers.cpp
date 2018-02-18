#include "globals.h"
#include <cassert>
using namespace std;

void scale2x(SDL_Surface* sf) {
	if (sf == NULL)  return;
	assert(sf->format->BitsPerPixel == 32);
	// init
	SDL_LockSurface(sf);
	uint32_t* data = (uint32_t*)sf->pixels;
	SDL_Rect dst = { 0, 0, 2, 2 };
	// loop through in reverse and blit
	for (int y = (sf->h-1)/2; y >= 0; y--)
	for (int x = (sf->w-1)/2; x >= 0; x--) {
		auto px = data[y * sf->w + x];
		dst.x = x*2,  dst.y = y*2;
		SDL_FillRect(sf, &dst, px);
	}
	SDL_UnlockSurface(sf);
}

void scale3x(SDL_Surface* sf) {
	if (sf == NULL)  return;
	assert(sf->format->BitsPerPixel == 32);
	// init
	SDL_LockSurface(sf);
	uint32_t* data = (uint32_t*)sf->pixels;
	SDL_Rect dst = { 0, 0, 3, 3 };
	// loop through in reverse and blit
	for (int y = sf->h/3-1; y >= 0; y--)
	for (int x = sf->w/3-1; x >= 0; x--) {
		auto px = data[y * sf->w + x];
		dst.x = x*3,  dst.y = y*3;
		SDL_FillRect(sf, &dst, px);
	}
	SDL_UnlockSurface(sf);
}

void scalex(SDL_Surface* sf, int sx) {
	if (sf == NULL)  return;
	assert(sx >= 1 && sx <= 5);
	assert(sf->format->BitsPerPixel == 32);
	// init
	SDL_LockSurface(sf);
	uint32_t* data = (uint32_t*)sf->pixels;
	SDL_Rect dst = { 0, 0, (uint16_t)sx, (uint16_t)sx };
	// loop through in reverse and blit
	for (int y = sf->h/sx-1; y >= 0; y--)
	for (int x = sf->w/sx-1; x >= 0; x--) {
		auto px = data[y * sf->w + x];
		dst.x = x*sx,  dst.y = y*sx;
		SDL_FillRect(sf, &dst, px);
	}
	SDL_UnlockSurface(sf);
}
