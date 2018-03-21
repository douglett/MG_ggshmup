#include "globals.h"
#include <cassert>
using namespace std;

namespace menus {
	int textspeed = 1;

	int init() {
		return 0;
	}
	
	void txtbox(SDL_Rect dst, const std::string& txt) {
		SDL_FillRect(buf, &dst, 0xffffffff);
		SDL_Rect dst2 = dst;  dst2.x++, dst2.y++, dst2.w-=2, dst2.h-=2;
		SDL_FillRect(buf, &dst2, 0x0000aaff);
		etc::qbprint(buf, dst.x+3, dst.y+3, txt.c_str());
	}
	
	void dialogue(const std::string& txt) {
		// init
		string txt2;
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
				anim = (anim+1) % textspeed;
				if (anim != 0)  ;
				else if (txt != txt2)
					txt2 += txt[ txt2.size() ];  // next letter
				else
					state = 2;  // eol - next state
			}
			// redraw
			//paint1();
			SDL_Rect dst = { 10, 100, 140, 40 };
			txtbox(dst, txt2);
			// flashing arrow
			if (state == 2) {
				anim = (anim+1) % 60;
				etc::qbprint(buf, dst.x+129, dst.y+31+anim/30, string()+char(31));
			}
			// display
			flip3x();
		}
	}
	
	std::string showlist(SDL_Rect mbox, const std::vector<std::string>& mitems) {
		//assert(mitems.size() && mitems.back() == "back");
		// init
		SDL_Surface* sf = etc::clonesurface(buf);
		int looping = 1;
		int arrow = 0;
		// action loop
		while (looping) {
			// main event loop
			SDL_Event e;
			while (SDL_PollEvent(&e))
				switch (e.type) {
					case SDL_QUIT:  exit(0);
					case SDL_KEYDOWN:
						switch (e.key.keysym.sym) {
							case SDLK_UP:     arrow = max(arrow-1, 0);  break;
							case SDLK_DOWN:   arrow = min(arrow+1, int(mitems.size()-1));  break;
							case SDLK_SPACE:  looping = 0;  break;
							default:  break;
						}
						break;
				}
			// redraw
			SDL_BlitSurface(sf, NULL, buf, NULL);
			string s;
			for (int i=0; i<mitems.size(); i++)
				s += (i == arrow ? char(16) : ' ') + mitems[i] + "\n";
			txtbox(mbox, s);
			// display
			flip3x();
		}
		SDL_FreeSurface(sf);
		return mitems[arrow];
	}
	
	void showinv() {
		vector<string> mitems = { "items", "quit", "back" };
		while (true) {
			paint1();
			battle::statsbox(105, 10);
			auto item = showlist({ 2, 10, 52, 29 }, mitems);
			if      (item == "back" )  break;
			else if (item == "quit" )  exit(0);
			else if (item == "items")  {
				while (true) {
					vector<string> mitems = { "back" };
					auto item = showlist({ 55+2, 10, 100, 120 }, mitems);
					if      (item == "back" )  break;
				}
			}
		}
	}
	
} // end menus
