#include "globals.h"
using namespace std;

SDL_Surface *buf, *tileset, *guy;
namespace viewport {
	int posx = 0,  posy = 0;
	int offx = 0,  offy = 0;
}


int main(int argc, char** argv) {
	#if defined(__WIN32__)
	freopen("CON", "wb", stdout);
	freopen("CON", "wb", stderr);
	#endif
	printf("hello world!\n");
	
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_SetVideoMode( 640, 480, 32, SDL_HWSURFACE );
	buf = SDL_CreateRGBSurface(SDL_SWSURFACE, 160, 144, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	tileset = loadbmp("rpgindoor1.bmp");
	guy = loadbmp("walker.bmp");
	map::loadmap("room1.tmx");
	
	npcs::npclist.push_back({ "guy", 5, 5, 0, 0 });
	npcs::npclist.push_back({ "guy", 6, 6, 0, 0 });
	
	mainloop();
	
	SDL_Quit();
	return 0;	
}


int mainloop() {
	SDL_Event e;
	int mainloop = 1;
	int movedir = -1;
	
	while (mainloop) {
		while (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				mainloop = 0;
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_UP:     movedir = 0;  break;
				case SDLK_RIGHT:  movedir = 1;  break;
				case SDLK_DOWN:   movedir = 2;  break;
				case SDLK_LEFT:   movedir = 3;  break;
				default:  ;
				}
			}
			else if (e.type == SDL_KEYUP) {
				switch (e.key.keysym.sym) {
				case SDLK_UP:     if (movedir == 0)  movedir = -1;  break;
				case SDLK_RIGHT:  if (movedir == 1)  movedir = -1;  break;
				case SDLK_DOWN:   if (movedir == 2)  movedir = -1;  break;
				case SDLK_LEFT:   if (movedir == 3)  movedir = -1;  break;
				default:  ;
				}
			}
			
		if (movedir >= 0)
			walk2(movedir);
				
		paint1();
		flip3x();
	}
	
	return 0;
}


void walk1(int dir) {
	// walkscreen pixels
	for (int i=1; i<16; i++) {
		switch (dir) {
			case 0:  viewport::offy++;  break;
			case 1:  viewport::offx--;  break;
			case 2:  viewport::offy--;  break;
			case 3:  viewport::offx++;  break;
		}
		paint1();
		flip3x();
	}
	// walkscreen tiles
	viewport::offy = viewport::offx = 0;
	switch (dir) {
		case 0:  viewport::posy--;  break;
		case 1:  viewport::posx++;  break;
		case 2:  viewport::posy++;  break;
		case 3:  viewport::posx--;  break;
	}
}

void walk2(int dir) {
	auto& n =  npcs::npclist[0];
	int x = n.x, y = n.y;
	switch (dir) {
		case 0:  y--;  break;
		case 1:  x++;  break;
		case 2:  y++;  break;
		case 3:  x--;  break;
	}
	if (x < 0 || y < 0 || x >= map::width || y >= map::height)  return;
	if (map::tmap[3][y * map::width + x] > 0)  return;
	// walk pixels
	for (int i=1; i<16; i++) {
		switch (dir) {
			case 0:  n.py--;  break;
			case 1:  n.px++;  break;
			case 2:  n.py++;  break;
			case 3:  n.px--;  break;
		}
		paint1();
		flip3x();
	}
	// walk tile
	n.px = n.py = 0;
	switch (dir) {
		case 0:  n.y--;  break;
		case 1:  n.x++;  break;
		case 2:  n.y++;  break;
		case 3:  n.x--;  break;
	}
}


void paint1() {
	// cls
	SDL_FillRect(buf, NULL, 0x111111ff);
	const int tswidth = tileset->w / 16;
	// loop each axis
	for (int y = -1; y <= 9; y++)
	for (int x = -1; x <= 10; x++) {
		if (viewport::posy + y < 0 || viewport::posy + y >= map::height)  continue;
		if (viewport::posx + x < 0 || viewport::posx + x >= map::width)  continue;
		// loop each layer
		for (int l = 0; l < map::layers-1; l++) {	
			int t = map::tmap[l][(viewport::posy + y) * map::width + (viewport::posx + x)];
			if (t == 0)  continue;
			t--;
			SDL_Rect src = { int16_t(t % tswidth * 16), int16_t(t / tswidth * 16), 16, 16 };
			SDL_Rect dst = { int16_t(x*16 + viewport::offx), int16_t(y*16 + viewport::offy), 0, 0 };
			SDL_BlitSurface(tileset, &src, buf, &dst);
		}
	}
	
	SDL_Rect src = { 0, 18*2, 16, 18 };
	for (const auto& n : npcs::npclist) {
		if (!npcs::inview(n))  continue;
		auto dst = npcs::getpos(n);
		SDL_BlitSurface(guy, &src, buf, &dst);
	}
}

