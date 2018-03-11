#include "globals.h"
#include <cassert>
#include <fstream>
#include <sstream>
using namespace std;


struct Map2D {
	int width = 0, height = 0, layers = 0;
	SDL_Surface* tileset = NULL;
	std::vector<std::vector<int>> tilemap;
	// tile map type overrides
	int loadmap(const std::string& fname);
	// functions
	int bounds(int l, int x, int y);
	int tileat(int l, int x, int y);
	int collide(int x, int y);
	SrcImg gettile(int l, int x, int y);
};
struct Map2DTmx : Map2D {
	loadmap(const std::string& fname);
};


//*** struct Map2D
int Map2D::bounds(int l, int x, int y) {
	return ( l < 0 || x < 0 || y < 0 || l >= layers || x >= width || y >= height ); 
}
	
int Map2D::tileat(int l, int x, int y) {
	if (bounds(l, x, y))  return 0;
	return tilemap[l][y * width + x];
}
	
int Map2D::collide(int x, int y) {
	if (layers < 2)  return 0;
	if (bounds(layers-1, x, y))  return 1;
	return tileat(layers-1, x, y);
}
	
SrcImg Map2D::gettile(int l, int x, int y) {
	assert(tileset != NULL);
	const int tswidth = tileset->w / 16;
	int t = tileat(l, x, y);
	if (t == 0)  return { {0}, NULL };
	t--;
	SDL_Rect r = { int16_t(t % tswidth * 16), int16_t(t / tswidth * 16), 16, 16 };
	return { r, tileset };
}
//*** end Map2D


//*** struct Map2DTmx	
int Map2DTmx::loadmap(const std::string& fname) {
	fstream fs(fname, fstream::in);
	assert(fs.is_open() == true);
	string input, s;
	while (getline(fs, s))
		input += s;
	fs.close();
	
	// get width and height
	width = height = 0;
	{
		int pos1 = input.find("width=\"") + 7;
		int pos2 = input.find("\"", pos1);
		s = input.substr(pos1, pos2-pos1);
		stringstream ss(s);
		ss >> width;
		//printf("[%d]\n", width);
	}
	{
		int pos1 = input.find("height=\"") + 8;
		int pos2 = input.find("\"", pos1);
		s = input.substr(pos1, pos2-pos1);
		stringstream ss(s);
		ss >> height;
		//printf("[%d]\n", height);
	}
	
	// get row data 1-3
	int lpos = 0;
	vector<string> data;
	while (true) {
		string start = "<data encoding=\"csv\">",  end = "</data>";
		int pos1 = input.find(start, lpos) ;
		if (pos1 == string::npos)  break;
		pos1 += start.length();
		int pos2 = input.find(end, pos1);
		lpos = pos2;
		s = input.substr(pos1, pos2-pos1);
		data.push_back(s);
		//printf("[%s]\n", s.c_str());
	}
	
	// parse rows to number list
	tilemap = {};
	for (auto& d : data) {
		for (char& c : d)
			if (c == ',')  c = ' ';
		tilemap.push_back({});
		stringstream ss(d);
		int n = 0;
		while (ss >> n)
			tilemap.back().push_back(n);
	}
	layers = tilemap.size();
	
	printf("map load OK:  %d,%d,%d\n", width, height, layers);
	return 0;
}
//*** end Map2DTmx


//*** struct Map2DAscii
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
		// copy into tilemap
		tilemap = { {}, {} };
		for (int y = 0; y < ascmap.size(); y++) {
			assert(ascmap[y].size() == width);  // better error checking here
			for (int x = 0; x < ascmap[y].size(); x++) {
				char c = ascmap[y][x];
				int t = 0, k = 0;
				if (chartile.count(c))  t = chartile.at(c);
				if (string("#TD").find(string()+c) != string::npos)  k = 1;
				tilemap[0].push_back(t);
				tilemap[1].push_back(k);
			}
		}
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
