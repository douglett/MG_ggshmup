#include "globals.h"
using namespace std;

namespace npcs {
	std::list<npc> npclist;
	static npc defnpc = { "none", "nilcoffee", 0 };  // default if nothing found
	
	int inview(const npc& n) {
		if (n.type == "nilcoffee")  return 0;
		if (n.x < viewport::posx-1 || n.x > viewport::posx+10 || n.y < viewport::posy-1 || n.y > viewport::posy+9)  return 0;
		return 1;
	}
	
	SDL_Rect getdst(const npc& n) {
		SDL_Rect r = { 0, 0, 0, 0 };
		r.x = (n.x - viewport::posx) * 16 - viewport::offx + n.px;
		r.y = (n.y - viewport::posy) * 16 - viewport::offy + n.py;
		return r;
	}
	
	SDL_Rect getsrc(const npc& n) {
		SDL_Rect r = { 16, 0, 16, 18 };
		r.y = 18 * n.dir;
		if (abs(n.px + n.py) > 7)  r.x += 16;
		return r;
	}
	
	npc& getbyid(const std::string& id) {
		for (auto& n : npclist)
			if (n.id == id)  return n;
		return defnpc;
	}
	
	npc& getbypos(int x, int y) {
		for (auto& n : npclist)
			if (n.x == x && n.y == y)  return n;
		return defnpc;
	}
	
	void erase(const npc& n) {
		for (auto it = npclist.begin(); it != npclist.end(); it++)
			if (&(*it) == &n) {
				npclist.erase(it);  // destroys iterator!
				break;
			}
	}
	void erase(const std::string id) {
		begin:
		for (auto it = npclist.begin(); it != npclist.end(); it++)
			if (it->id == id) {
				npclist.erase(it);  // destroys iterator!
				goto begin;  // iterator invalidated, so return to start
			}
	}
}

