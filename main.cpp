#include "globals.h"
using namespace std;

SDL_Surface *buf, *tileset;
vector<string> gmap = {
	"##########",
	".,.,.,.,.,",
	",.,.,.,.,.",
	".,.,.,.,.,",
	",.,.,.,.,.",
	".,.,.,.,.,",
	",.,.,.,.,.,.,.,.",
	".,.,.,.,.,",
	",.,.#.,.,.",
	".,.,.,.,.,",
	",.,.,.,.,.",
	".,.,.,.,.,",
	",.,.,.,.,.",
	".,.,.,.,.#",
};
namespace viewport {
	int posx = 1,  posy = 0;
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
	
	mainloop();
	
	SDL_Quit();
	return 0;	
}


int mainloop() {
	SDL_Event e;
	int mainloop = 1;
	
	while (mainloop) {
		while (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				mainloop = 0;
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
//				case SDLK_UP:     viewport::posy--;  break;
//				case SDLK_RIGHT:  viewport::posx++;  break;
//				case SDLK_DOWN:   viewport::posy++;  break;
//				case SDLK_LEFT:   viewport::posx--;  break;
				case SDLK_UP:     walk(0);  break;
				case SDLK_RIGHT:  walk(1);  break;
				case SDLK_DOWN:   walk(2);  break;
				case SDLK_LEFT:   walk(3);  break;
				default:  ;
				}
			}
				
		paint1();
		flip3x();
	}
	
	return 0;
}


void walk(int dir) {
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
	viewport::offy = viewport::offx = 0;
	switch (dir) {
		case 0:  viewport::posy--;  break;
		case 1:  viewport::posx++;  break;
		case 2:  viewport::posy++;  break;
		case 3:  viewport::posx--;  break;
	}
}


void paint1() {
	SDL_FillRect(buf, NULL, 0x111111ff);
//	SDL_Rect r = { 16, 16, 16, 16 };
//	SDL_FillRect(buf, &r, 0x00ff00ff);
	
//	SDL_BlitSurface(tileset, &r, buf, &r);
	
	for (int y = -1; y <= 9; y++)
	for (int x = -1; x <= 10; x++) {
		if (viewport::posy + y < 0 || viewport::posy + y >= gmap.size())  continue;
		if (viewport::posx + x < 0 || viewport::posx + x >= gmap[viewport::posy + y].size())  continue;
		uint32_t c = 0;
		char t = gmap[viewport::posy + y][viewport::posx + x];
		switch (t) {
		case ' ':  continue;
		case '#':  c = 0x999999ff;  break;
		case '.':  c = 0x00aa00ff;  break;
		case ',':  c = 0x008800ff;  break;
		}
		SDL_Rect r = { 0, 0, 16, 16 };
		r.x = x*16 + viewport::offx,  r.y = y*16 + viewport::offy;
		SDL_FillRect(buf, &r, c);
	}
	
	SDL_Rect src = { 16*6, 0, 16*5, 16*2 };
	SDL_Rect dst = { 16, 16, 0, 0 };
	SDL_BlitSurface(tileset, &src, buf, &dst);
}

void flip3x() {
	SDL_Rect bufpos = { 24, 8, 0, 0 };
	SDL_FillRect( SDL_GetVideoSurface(), NULL, 0x0 );
	SDL_BlitSurface( buf, NULL, SDL_GetVideoSurface(), &bufpos );
	scalex( SDL_GetVideoSurface(), 3 );
	SDL_Flip( SDL_GetVideoSurface() );
	SDL_Delay(16);
}

