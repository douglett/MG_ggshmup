#pragma once
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <SDL/SDL.h>

extern SDL_Surface *buf, *tileset, *guy;
int  mainloop();
void walk1(int dir);
void walk2(int dir);
void paint1();

// helpers
SDL_Surface* loadbmp(const std::string& fname);
void scalex(SDL_Surface* sf, int sx);
void flip3x();

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
		std::string id;
		int x, y;
		int px, py;
	};
	extern std::vector<npc> npclist;
	int inview(const npc& n);
	SDL_Rect getpos(const npc& n);
}
