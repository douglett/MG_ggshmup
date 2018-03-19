#include "globals.h"
#include <algorithm>
#include <cassert>
using namespace std;

SDL_Surface *buf, *tileset, *guy, *guyshadow, *idsquare, *qbfont;


int main(int argc, char** argv) {
	#if defined(__WIN32__)
	freopen("CON", "wb", stdout);
	freopen("CON", "wb", stderr);
	#endif
	printf("hello world!\n");
	
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_SetVideoMode( 640, 480, 32, SDL_HWSURFACE );
	buf = SDL_CreateRGBSurface(SDL_SWSURFACE, 160, 144, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	menus::init();
	qbfont = loadbmp("res/qbfont.bmp");
	guy = loadbmp("res/walker.bmp");
	create_help_sprites();
//	tileset = loadbmp("rpgindoor1.bmp");
//	map::loadmap("room1.tmx");
	tileset = loadbmp("res/hicontile.bmp");
	gmap::loadascii("mil");
	
	gmap::spritelist.push_back({ "guy", {5*16, 5*16, 16, 16}, {{16, 18*2, 16, 18}, guy} });
	gmap::spritelist.push_back({ "test1", {3*16, 3*16, 16, 16}, {{0}, NULL} });
	gmap::spritelist.push_back({ "test2", {2*16, 3*16, 16, 16}, {{16, 18*2, 16, 18}, guy} });
	
	mainloop();
	
	SDL_Quit();
	return 0;	
}

void create_help_sprites() {
	// create npc shadow
	SDL_Surface* sf = mksurface(16, 16);
	SDL_FillRect(sf, NULL, 0xff00ff00);
	for (auto& r : vector<SDL_Rect>{ {2, 10, 12, 2}, {3, 9, 10, 4}, {4, 8, 8, 6} })
		SDL_FillRect(sf, &r, 0x00000088);
	guyshadow = sf;
	// create indicator square
	sf = mksurface(16, 16);
	SDL_FillRect(sf, NULL, 0xffaa0077);
	idsquare = sf;
}


int mainloop() {
	SDL_Event e;
	int mainloop = 1;
	int dir = 2, domove = 0;
	gmap::getsprite("guy")->img.r.y = dir*18;  // set first direction
	
	while (mainloop) {
		while (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				mainloop = 0;
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_UP:     dir = 0, domove = 1;  break;
				case SDLK_RIGHT:  dir = 1, domove = 1;  break;
				case SDLK_DOWN:   dir = 2, domove = 1;  break;
				case SDLK_LEFT:   dir = 3, domove = 1;  break;
				case SDLK_SPACE:  action2(dir);  break;
				case 's':
				case 'i':         menus::showinv();  break;
				default:  ;
				}
			}
			else if (e.type == SDL_KEYUP) {
				switch (e.key.keysym.sym) {
				case SDLK_UP:     if (dir == 0)  domove = 0;  break;
				case SDLK_RIGHT:  if (dir == 1)  domove = 0;  break;
				case SDLK_DOWN:   if (dir == 2)  domove = 0;  break;
				case SDLK_LEFT:   if (dir == 3)  domove = 0;  break;
				default:  ;
				}
			}
		
		if (domove) {
			walk2(dir);
			if (action2(-1))  domove = 0;
		}
		recenter();
		paint1();
		flip3x();
	}
	
	return 0;
}

void walk2(int dir) {
	assert(gmap::getsprite("guy") != NULL);
	auto& spr = *gmap::getsprite("guy");
	spr.img.r.y = dir*18;
	int x = spr.pos.x/16, y = spr.pos.y/16;
	switch (dir) {
		case 0:  y--;  break;
		case 1:  x++;  break;
		case 2:  y++;  break;
		case 3:  x--;  break;
	}
	// collision 1
	if (gmap::collide(x, y))  return;
	// walk pixels
	for (int i=1; i<=16; i++) {
		switch (dir) {
			case 0:  spr.pos.y--;  break;
			case 1:  spr.pos.x++;  break;
			case 2:  spr.pos.y++;  break;
			case 3:  spr.pos.x--;  break;
		}
		if (i == 7)  spr.img.r.x = 16*2;
		recenter();
		paint1();
		flip3x();
	}
	// snap to tile
	spr.img.r.x = 16*1;
	spr.pos.x = round(spr.pos.x/16.0)*16;
	spr.pos.y = round(spr.pos.y/16.0)*16;
}


void walk3(int dir) {
	switch (dir) {
		case 0:  gmap::viewport.y--;  break;
		case 1:  gmap::viewport.x++;  break;
		case 2:  gmap::viewport.y++;  break;
		case 3:  gmap::viewport.x--;  break;
	}
}


int action2(int dir) {
	assert(gmap::getsprite("guy") != NULL);
	auto& spr = *gmap::getsprite("guy");
	int x = spr.pos.x/16, y = spr.pos.y/16;
	switch (dir) {
		case 0:  y--;  break;
		case 1:  x++;  break;
		case 2:  y++;  break;
		case 3:  x--;  break;
	}
	// get npc
	gmap::Sprite* aspr = NULL;
	for (auto& spr : gmap::spritelist)
		if (spr.id != "guy" && spr.pos.x/16 == x && spr.pos.y/16 == y) {
			aspr = &spr;
			break;
		}
	if (aspr == NULL)  return 0;
	printf("npc found: [%s]\n", aspr->id.c_str());
	// action
	return npcs::action("", aspr);
}


void recenter() {
	const auto* spr = gmap::getsprite("guy");
	assert(spr != NULL);
	gmap::viewport.x = spr->pos.x - 4*16 - 7;
	gmap::viewport.y = spr->pos.y - 4*16;
}


//static bool npc_sort_posy(const npcs::npc& l, const npcs::npc& r) {
//	return (l.y < r.y);
//}

void paint1() {
	// cls
	SDL_FillRect(buf, NULL, 0x111111ff);
	// repaint map
	gmap::paint();
	qbprint(buf, 1, 1, "RPG proto");  // test info
}

void flip3x() {
	// scale backbuffer and flip screen
	SDL_Rect bufpos = { 24, 8, 0, 0 };
	SDL_FillRect( SDL_GetVideoSurface(), NULL, 0x0 );
	SDL_BlitSurface( buf, NULL, SDL_GetVideoSurface(), &bufpos );
	scalex( SDL_GetVideoSurface(), 3 );
	SDL_Flip( SDL_GetVideoSurface() );
	SDL_Delay(16);
}

