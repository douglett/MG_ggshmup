#include "globals.h"
using namespace std;

struct Map2D {
	int width = 0, height = 0, layers = 0;
	SDL_Surface* tileset = NULL;
	// tile map type overrides
	int loadmap(const std::string& fname);
	int tileat(int l, int x, int y);
	int collide(int x, int y);
	
	int bounds(int l, int x, int y) {
		return ( l < 0 || x < 0 || y < 0 || l >= layers || x >= width || y >= height ); 
	}
	
	SrcImg gettile(int l, int x, int y) {
		const int tswidth = tileset->w / 16;
		int t = tileat(l, x, y);
		if (t == 0)  return { {0}, NULL };
		t--;
		SDL_Rect r = { int16_t(t % tswidth * 16), int16_t(t / tswidth * 16), 16, 16 };
		return { r, tileset };
	}
};


struct Map2DTmx : Map2D {
	std::vector<std::vector<int>> tmap;
	
	int loadmap(const std::string& fname) {
		return 1;
	}
	
	int tileat(int l, int x, int y) {
		if (bounds(l, x, y))  return 0;
		return tmap[l][y * width + x];
	}
	
	int collide(int x, int y) {
		if (bounds(layers-1, x, y))  return 1;
		return 0;
	}
};


struct Map2DAscii : Map2D {
	std::vector<std::string> ascmap;
	const std::map<char, int> chartile = {
		{ ' ',  0 },
		{ ',',  1 },
		{ '.',  3 },
		{ '#',  8 },
		{ 'T',  9 },
		{ 'D', 10 },
		{ '/', 11 },
		{ '|', 12 }
	};
	
	int loadmap(const std::string& fname) {
		ascmap = {
			"############",
			"#..#.......#",
			"#..#.......#",
			"#..........#",
			"#####..#####",
			"#/........|#",
			"#..........#",
			"#..........#",
			"#####DD#####",
			"T,,,,,,,,,,T",
			"T,,,,,,,,,,T",
		};
		height = ascmap.size();
		width = ascmap[0].size();
		return 0;
	}
	
	char tileascii(int x, int y) {
		if (bounds(0, x, y))  return 0;
		return ascmap[y][x];
	}
	
	int tileat(int l, int x, int y) {
		return chartile.at( tileascii(x, y) );
	}
	
	int collide(int x, int y) {
		if (bounds(0, x, y))  return 1;
		return 0;
	}
};
