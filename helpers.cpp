#include "globals.h"
#include <cassert>
using namespace std;

SDL_Surface* loadbmp(const std::string& fname) {
	// load and convert to match backbuffer
	SDL_Surface* t1 = SDL_LoadBMP(fname.c_str());
	SDL_Surface* t2 = SDL_ConvertSurface(t1, buf->format, buf->flags);
	SDL_FreeSurface(t1);
	// set transparency
	SDL_SetColorKey(t2, SDL_SRCCOLORKEY, 0xff00ffff);  // method 1
	uint32_t* data = (uint32_t*)t2->pixels;  // method 2
	for (int i = 0; i < t2->w * t2->h; i++)
		if (data[i] == 0xff00ffff)  data[i] = 0xff00ff00;
	return t2;
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

void flip3x() {
	// scale backbuffer and flip screen
	SDL_Rect bufpos = { 24, 8, 0, 0 };
	SDL_FillRect( SDL_GetVideoSurface(), NULL, 0x0 );
	SDL_BlitSurface( buf, NULL, SDL_GetVideoSurface(), &bufpos );
	scalex( SDL_GetVideoSurface(), 3 );
	SDL_Flip( SDL_GetVideoSurface() );
	SDL_Delay(16);
}

