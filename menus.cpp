#include "globals.h"
using namespace std;

namespace menus {

	int init() {
		return 0;
	}
	
	void dialogue(const std::vector<std::string>& lines) {
		// init
		vector<string> l2 = { "" };
		int state = 1;
		int anim = 0;
		// action loop
		while (state > 0) {
			// main event loop
			SDL_Event e;
			while (SDL_PollEvent(&e))
				switch (e.type) {
					case SDL_QUIT:  exit(0);
					case SDL_KEYDOWN:
						if (state != 2)  continue;
						switch (e.key.keysym.sym) {
							case SDLK_SPACE:  state = 0;  break;
							default:  break;
						}
						break;
				}
			// show text
			if (state == 1) {
				anim = (anim+1) % 3;
				if (anim != 0)  ;
				else if (l2.back().size() < lines[ l2.size()-1 ].size()) 
					l2.back() += lines[ l2.size()-1 ][ l2.back().size() ];  // add one letter to current line
				else if (l2.back() == lines[ l2.size()-1 ]) {
					if    (l2.size() < lines.size())  l2.push_back("");  // next line
					else  state = 2;  // eol - next state
				}
			}
			// redraw
			paint1();
			SDL_Rect dst = { 10, 100, 140, 40 };
			SDL_FillRect(buf, &dst, 0xffffffff);
			SDL_Rect dst2 = dst;  dst2.x++, dst2.y++, dst2.w-=2, dst2.h-=2;
			SDL_FillRect(buf, &dst2, 0x0000aaff);
			for (int i=0; i<min((int)l2.size(), 3); i++)
				qbprint(buf, dst.x+3, dst.y+3+8*i, l2[i]);
			// flashing arrow
			if (state == 2) {
				anim = (anim+1) % 60;
				qbprint(buf, dst.x+129, dst.y+31+anim/30, string()+char(31));
			}
			// display
			flip3x();
		}
	}
	
	void showinv() {
		// init
		int looping = 1;
		int anim = 0;
		// action loop
		while (looping) {
			// main event loop
			SDL_Event e;
			while (SDL_PollEvent(&e))
				switch (e.type) {
					case SDL_QUIT:  exit(0);
					case SDL_KEYDOWN:
						switch (e.key.keysym.sym) {
							case SDLK_SPACE:  looping = 0;  break;
							default:  break;
						}
						break;
				}
			// redraw
			paint1();
			// flashing arrow
			SDL_Rect dst = { 10, 100, 140, 40 };
			anim = (anim+1) % 60;
			qbprint(buf, dst.x+129, dst.y+31+anim/30, string()+char(31));
			// display
			flip3x();
		}
	}
	
} // end menus
