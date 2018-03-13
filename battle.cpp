#include "globals.h"

namespace battle {
	
	void begin() {
		SDL_Surface* sf = clonesurface(buf);
		printf("{%d} %d %d\n", sf == NULL, sf->w, sf->h);
		int looping = 1;
		while (looping) {
			// main event loop
			SDL_Event e;
			while (SDL_PollEvent(&e))
			switch (e.type) {
				case SDL_QUIT:  exit(0);
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym) {
						default:  break;
					}
					break;
			}
			// redraw
			SDL_BlitSurface(sf, NULL, buf, NULL);
			SDL_Rect dst = { 30, 20, 100, 60 };
			SDL_FillRect(buf, &dst, 0xffffffff);
			dst.x++, dst.y++, dst.w-=2, dst.h-=2;
			SDL_FillRect(buf, &dst, 0x000000ff);
//			flip3x();
			// main menu
			auto opt = menus::showlist({ 10, 90, 60, 4+8*3 }, {"fight", "magic", "flee"});
			if (opt == "flee")  looping = 0;
			else if (opt == "fight") {
				menus::dialogue("you slice the\nmonster for 2!");
				menus::dialogue("monster misses!");
			}
		}
		SDL_FreeSurface(sf);
	}
	
} // end battle
