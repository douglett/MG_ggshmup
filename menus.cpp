#include "globals.h"
using namespace std;

namespace menus {

	int init() {
		return 0;
	}
	
	void txtbox(SDL_Rect dst, const std::string& txt) {
		SDL_FillRect(buf, &dst, 0xffffffff);
		SDL_Rect dst2 = dst;  dst2.x++, dst2.y++, dst2.w-=2, dst2.h-=2;
		SDL_FillRect(buf, &dst2, 0x0000aaff);
		qbprint(buf, dst.x+3, dst.y+3, txt.c_str());
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
				anim = (anim+1) % 3;
				if (anim != 0)  ;
				else if (txt != txt2)
					txt2 += txt[ txt2.size() ];  // next letter
				else
					state = 2;  // eol - next state
			}
			// redraw
			paint1();
			SDL_Rect dst = { 10, 100, 140, 40 };
			txtbox(dst, txt2);
			// flashing arrow
			if (state == 2) {
				anim = (anim+1) % 60;
				qbprint(buf, dst.x+129, dst.y+31+anim/30, string()+char(31));
			}
			// display
			flip3x();
		}
	}
	
//	static std::string join(const std::vector<std::string>& vs, const std::string& glue) {
//		string s;
//		for (int i=0; i<vs.size(); i++)
//			s += ( i == 0 ? "" : glue ) + vs[i];
//		return s;
//	}
	
	void showinv() {
		// init
		int looping = 1;
		int arrow = 0;
		//int anim = 0;
		vector<string> mitems = { "items", "quit", "back" };
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
							case SDLK_SPACE:
								if      (mitems[arrow] == "back" )  looping = 0;
								else if (mitems[arrow] == "quit" )  exit(0);
								else if (mitems[arrow] == "items")  ;
								break;
							default:  break;
						}
						break;
				}
			// redraw
			paint1();
			SDL_Rect dst = { 2, 10, uint16_t(8*6 + 5), uint16_t(8*mitems.size() + 6) };
			string s;
			for (int i=0; i<mitems.size(); i++)
				s += (i == arrow ? char(16) : ' ') + mitems[i] + "\n";
			txtbox(dst, s);
			// flashing arrow
			//anim = (anim+1) % 60;
			//qbprint(buf, dst.x+3 + anim/30, dst.y+arrow*8+2, string()+char(16));
			// display
			flip3x();
		}
	}
	
} // end menus
