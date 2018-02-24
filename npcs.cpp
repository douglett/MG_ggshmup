#include "globals.h"
using namespace std;

namespace npcs {
	std::vector<npc> npclist;
	
	int inview(const npc& n) {
		if (n.x < viewport::posx-1 || n.x > viewport::posx+10 || n.y < viewport::posy-1 || n.y > viewport::posy+9)
			return 0;
		return 1;
	}
	
	SDL_Rect getpos(const npc& n) {
		SDL_Rect r = { 0, 0, 0, 0 };
		r.x = (n.x - viewport::posx) * 16 + viewport::offx;
		r.y = (n.y - viewport::posy) * 16 + viewport::offy - 6;
		return r;
	}
}

