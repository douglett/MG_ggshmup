#pragma once
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <SDL/SDL.h>

// types
struct SrcImg { SDL_Rect r; SDL_Surface* sf; };

// main
extern SDL_Surface *buf, *tileset, *guy, *guyshadow, *qbfont;
int  mainloop();
SDL_Surface* createshadow();
void walk1(int dir);
void walk2(int dir);
void walk3(int dir);
void action1();
void action2();
void paint1();
void flip3x();

// helpers
SDL_Surface* loadbmp(const std::string& fname);
SDL_Surface* mksurface(int w, int h);
SDL_Surface* clonesurface(SDL_Surface* sf);
void scalex(SDL_Surface* sf, int sx);
void qbprint(SDL_Surface* sf, int x, int y, const std::string& s);
std::string join(const std::vector<std::string>& vs, const std::string& glue);

namespace viewport {
	extern int posx, posy;
	extern int offx, offy;
	extern std::string follow;
	void recenter();
}
namespace gmap {
	extern int width, height, layers;
	extern SDL_Rect viewport;
	extern std::vector<std::vector<int>> tilemap;
	struct Sprite { std::string id; SDL_Rect pos; SrcImg img; };
	extern std::list<gmap::Sprite> spritelist;
	int loadmap(const std::string& fname);
	int loadascii(const std::string& fname);
	int bounds(int l, int x, int y);
	int collide(int x, int y);
	SrcImg gettile(int l, int x, int y);
	void paint(SDL_Rect viewport, int posx, int posy);
	void paint();
}
namespace npcs {
	struct npc {
		std::string id, type;
		int x, y;
		int px, py;
		int dir;
	};
	extern std::list<npc> npclist;
	int inview(const npc& n);
	SDL_Rect getdst(const npc& n);
	SDL_Rect getsrc(const npc& n);
	npc& getbyid(const std::string& id);
	npc& getbypos(int x, int y);
	void erase(const npc& n);
	void erase(const std::string id);
}
namespace menus {
	extern int textspeed;
	int init();
	void txtbox(SDL_Rect dst, const std::string& txt);
	void dialogue(const std::string& str);
	std::string showlist(SDL_Rect mbox, const std::vector<std::string>& mitems);
	void showinv();
}
namespace battle {
	struct BattleStats {
		std::string name;
		int str, intl, stm;
		int lvl, elem;
		int hp, mp;
	};
	extern BattleStats player, enemy;
	void rest(BattleStats& st);
	void statsbox(int x, int y);
	void begin();
	void exchange(int pdmg, int pelem);
}
