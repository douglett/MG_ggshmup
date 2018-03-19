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

SDL_Surface* mksurface(int w, int h) {
	SDL_Surface* sf = SDL_CreateRGBSurface(buf->flags, w, h, buf->format->BitsPerPixel, 
		buf->format->Rmask, buf->format->Gmask, buf->format->Bmask, buf->format->Amask);
	SDL_FillRect(sf, NULL, 0x000000ff);
	return sf;
}

SDL_Surface* clonesurface(SDL_Surface* sf) {
	assert(sf != NULL);
	SDL_Surface* sfnew = mksurface(sf->w, sf->h);
	SDL_BlitSurface(sf, NULL, sfnew, NULL);
	return sfnew;
}

void scalex(SDL_Surface* sf, int sx) {
	assert(sf != NULL);
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

void qbprint(SDL_Surface* sf, int x, int y, const std::string& s) {
	assert(sf != NULL);
	const int pitch = qbfont->w/8;
	SDL_Rect src = { 0, 0, 8, 8 },  dst = { int16_t(x), int16_t(y), 8, 8 };
	for (int i=0; i<s.size(); i++) {
		if (s[i] == '\n') {
			dst.x = x, dst.y += 8; 
			continue;
		}
		src.x = s[i] % pitch * 8;
		src.y = s[i] / pitch * 8;
		SDL_BlitSurface(qbfont, &src, sf, &dst);
		dst.x += 8;
	}
}

std::string join(const std::vector<std::string>& vs, const std::string& glue) {
	string s;
	for (int i=0; i<vs.size(); i++)
		s += ( i == 0 ? "" : glue ) + vs[i];
	return s;
}
