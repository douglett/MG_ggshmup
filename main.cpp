#include "globals.h"
using namespace std;

SDL_Surface *buf = NULL;


int main(int argc, char** argv) {
	#if defined(__WIN32__)
	freopen("CON", "wb", stdout);
	freopen("CON", "wb", stderr);
	#endif
	printf("hello world!\n");
	
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_SetVideoMode( 640, 480, 32, SDL_HWSURFACE );
	buf = SDL_CreateRGBSurface(SDL_SWSURFACE, 160, 144, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	
	mainloop();
	
	SDL_Quit();
	return 0;	
}


int mainloop() {
	SDL_Rect bufpos = { 24, 8, 0, 0 };
	SDL_Event e;
	int mainloop = 1;
	
	while (mainloop) {
		while (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				mainloop = 0;
				
		paint1();
		
		SDL_FillRect( SDL_GetVideoSurface(), NULL, 0x0 );
		SDL_BlitSurface( buf, NULL, SDL_GetVideoSurface(), &bufpos );
		scalex( SDL_GetVideoSurface(), 3 );
		SDL_Flip( SDL_GetVideoSurface() );
	}
	
	return 0;
}


void paint1() {
	SDL_FillRect(buf, NULL, 0xff0000ff);
	SDL_Rect r = { 16, 16, 16, 16 };
	SDL_FillRect(buf, &r, 0x00ff00ff);
}

