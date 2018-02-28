#include "globals.h"
#include <algorithm>
using namespace std;

SDL_Surface *buf, *tileset, *guy, *guyshadow, *qbfont;
namespace viewport {
	int posx = 0,  posy = 0;
	int offx = 0,  offy = 0;
}


int main(int argc, char** argv) {
	#if defined(__WIN32__)
	freopen("CON", "wb", stdout);
	freopen("CON", "wb", stderr);
	#endif
	printf("hello world!\n");
	
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_SetVideoMode( 640, 480, 32, SDL_HWSURFACE );
	buf = SDL_CreateRGBSurface(SDL_SWSURFACE, 160, 144, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	tileset = loadbmp("rpgindoor1.bmp");
	guy = loadbmp("walker.bmp");
	guyshadow = createshadow();
	qbfont = loadbmp("qbfont.bmp");
	menus::init();
	map::loadmap("room1.tmx");
	
	npcs::npclist.push_back({ "guy",    "walker", 5, 5, 0, 0, 2 });
	npcs::npclist.push_back({ "npc1",   "walker", 6, 3, 0, 0, 2 });
	npcs::npclist.push_back({ "coffee", "coffee", 1, 2 });
	npcs::npclist.push_back({ "book",   "coffee", 0, 3 });
	
	mainloop();
	
	SDL_Quit();
	return 0;	
}

SDL_Surface* createshadow() {
	SDL_Surface* sf = mksurface(16, 16);
	for (auto& r : vector<SDL_Rect>{ {2, 10, 12, 2}, {3, 9, 10, 4}, {4, 8, 8, 6} })
		SDL_FillRect(sf, &r, 0x00000088);
	return sf;
}


int mainloop() {
	SDL_Event e;
	int mainloop = 1;
	int movedir = -1;
	
	while (mainloop) {
		while (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				mainloop = 0;
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_UP:     movedir = 0;  break;
				case SDLK_RIGHT:  movedir = 1;  break;
				case SDLK_DOWN:   movedir = 2;  break;
				case SDLK_LEFT:   movedir = 3;  break;
				case SDLK_SPACE:  action1();  break;
				case 's':
				case 'i':         menus::showinv();  break;
				default:  ;
				}
			}
			else if (e.type == SDL_KEYUP) {
				switch (e.key.keysym.sym) {
				case SDLK_UP:     if (movedir == 0)  movedir = -1;  break;
				case SDLK_RIGHT:  if (movedir == 1)  movedir = -1;  break;
				case SDLK_DOWN:   if (movedir == 2)  movedir = -1;  break;
				case SDLK_LEFT:   if (movedir == 3)  movedir = -1;  break;
				default:  ;
				}
			}
			
		if (movedir >= 0)
			walk2(movedir);
				
		paint1();
		flip3x();
	}
	
	return 0;
}


void walk1(int dir) {
	// walkscreen pixels
	for (int i=1; i<16; i++) {
		switch (dir) {
			case 0:  viewport::offy++;  break;
			case 1:  viewport::offx--;  break;
			case 2:  viewport::offy--;  break;
			case 3:  viewport::offx++;  break;
		}
		paint1();
		flip3x();
	}
	// walkscreen tiles
	viewport::offy = viewport::offx = 0;
	switch (dir) {
		case 0:  viewport::posy--;  break;
		case 1:  viewport::posx++;  break;
		case 2:  viewport::posy++;  break;
		case 3:  viewport::posx--;  break;
	}
}

void walk2(int dir) {
	auto& n =  npcs::getbyid("guy");
	int x = n.x, y = n.y;
	switch (dir) {
		case 0:  y--;  n.dir = 0;  break;
		case 1:  x++;  n.dir = 1;  break;
		case 2:  y++;  n.dir = 2;  break;
		case 3:  x--;  n.dir = 3;  break;
	}
	// bounds check
	if (x < 0 || y < 0 || x >= map::width || y >= map::height)  return;
	// collision 1
	if (map::tmap[map::layers-1][y * map::width + x] > 0)  return;
	// collision 2
	for (const auto& nn : npcs::npclist)
		if (nn.x == x && nn.y == y)  return;
	// walk pixels
	for (int i=1; i<16; i++) {
		switch (dir) {
			case 0:  n.py--;  break;
			case 1:  n.px++;  break;
			case 2:  n.py++;  break;
			case 3:  n.px--;  break;
		}
		paint1();
		flip3x();
	}
	// walk tile
	n.px = n.py = 0;
	switch (dir) {
		case 0:  n.y--;  break;
		case 1:  n.x++;  break;
		case 2:  n.y++;  break;
		case 3:  n.x--;  break;
	}
}


void action1() {
	auto& n = npcs::getbyid("guy");
	int x = n.x, y = n.y;
	switch (n.dir) {
		case 0:  y--;  break;
		case 1:  x++;  break;
		case 2:  y++;  break;
		case 3:  x--;  break;
	}
	for (const auto& nn : npcs::npclist) {
		if (nn.x != x || nn.y != y)  continue;
		printf("[[%s]]\n", nn.id.c_str());  // show id
		if (nn.id == "npc1") {
			menus::dialogue("hello, how are\nyou?");
			menus::dialogue("i hope you are\nwell!");
		}
		else if (nn.id == "coffee") {
			menus::dialogue("you sip some\ncoffee.\nmmm!");
		}
		else if (nn.id == "book") {
			menus::dialogue("\"to be or not to\nbe;\"\na gripping read!");
		}
		break;
	}
}


static bool npcsort(const npcs::npc& l, const npcs::npc& r) {
	return (l.y < r.y);
}

void paint1() {
	// cls
	SDL_FillRect(buf, NULL, 0x111111ff);
	const int tswidth = tileset->w / 16;
	// loop each axis
	for (int y = -1; y <= 9; y++)
	for (int x = -1; x <= 10; x++) {
		if (viewport::posy + y < 0 || viewport::posy + y >= map::height)  continue;
		if (viewport::posx + x < 0 || viewport::posx + x >= map::width)  continue;
		// loop each layer
		for (int l = 0; l < map::layers-1; l++) {	
			int t = map::tmap[l][(viewport::posy + y) * map::width + (viewport::posx + x)];
			if (t == 0)  continue;
			t--;
			SDL_Rect src = { int16_t(t % tswidth * 16), int16_t(t / tswidth * 16), 16, 16 };
			SDL_Rect dst = { int16_t(x*16 + viewport::offx), int16_t(y*16 + viewport::offy), 0, 0 };
			SDL_BlitSurface(tileset, &src, buf, &dst);
		}
	}
	// draw npcs
	auto nls = npcs::npclist;
	sort(nls.begin(), nls.end(), npcsort);
	for (const auto& n : nls) {
		if (!npcs::inview(n))  continue;
		auto src = npcs::getsrc(n);
		auto dst = npcs::getpos(n);
		SDL_BlitSurface(guyshadow, NULL, buf, &dst);
		dst.y -= 6;
		SDL_BlitSurface(guy, &src, buf, &dst);
	}
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

