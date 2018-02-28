#pragma once
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <SDL/SDL.h>

extern SDL_Surface *buf, *tileset, *guy, *guyshadow, *qbfont;
int  mainloop();
SDL_Surface* createshadow();
void walk1(int dir);
void walk2(int dir);
void action1();
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
}
namespace map {
	extern int width, height, layers;
	extern std::vector<std::vector<int>> tmap;
	int loadmap(const std::string& fname);
}
namespace npcs {
	struct npc {
		std::string id, type;
		int x, y;
		int px, py;
		int dir;
	};
	extern std::vector<npc> npclist;
	int inview(const npc& n);
	SDL_Rect getpos(const npc& n);
	SDL_Rect getsrc(const npc& n);
	npc& getbyid(const std::string& id);
}

namespace menus {
	int init();
	void txtbox(SDL_Rect dst, const std::string& txt);
	void dialogue(const std::string& str);
	std::string showlist(SDL_Rect mbox, const std::vector<std::string>& mitems);
	void showinv();
}
