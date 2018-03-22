#include "globals.h"
using namespace std;

namespace mygame {

	int init() {
		// castle map
		gmap::loadascii({
			"TTTTT,,TTTTT",
			"TTTTT,TTTTTT",
			"TTTTT,TTTTTT",
			"TTTTT,TTTTTT",
			"TTTTT,,TTTTT",
			"TTTTT,,TTTTT",
			"T,,TT,,TT,TT",
			"T,,,,,,,,,,T",
			"T,,,,,,,,,,T",
			"T,,,##,,,,,T",
			"#####DD#####",
			"#.....##...#",
			"#..........#",
			"#####..#####",
			"#/..#..#..|#",
			"#..........#",
			"############"
		});
		// set npc sprites
		etc::SrcImg guy1 = {{16, 18*2, 16, 18}, guy}, nil1 = {{0}, NULL};
		int ym = gmap::height-1;
		gmap::spritelist.push_back({ "guy",      {5*16, int16_t((ym-1)*16), 16, 16}, guy1 });
		gmap::spritelist.push_back({ "secret1",  {1*16, int16_t((ym-5)*16), 16, 16}, nil1 });
		gmap::spritelist.push_back({ "test2",    {5*16, int16_t((ym-5)*16), 16, 16}, guy1 });
		//
		battle::rest(battle::player);
		return 0;
	}
	
	int action(const std::string& mapname, gmap::Sprite* spr) {
		if (spr == NULL)  
			return 0;
		else if (spr->id == "test2") {
			menus::dialogue("Halt! Return to\nyour cell or die!");
			paint1();
			battle::begin();
			if (battle::player.hp == 0)  exit(0);
			if (battle::enemy.hp == 0)  gmap::delsprite(spr);
		}
		else if (spr->id == "secret1") {
			string item = "h-potion";
			menus::dialogue("You found a\n"+item+"!");
			menus::items.push_back(item);
		}
		else if (spr->id == "door1") {
			int pos = spr->pos.y/16 * gmap::width + spr->pos.x/16;
			gmap::tilemap[0][pos] = 0;
			gmap::tilemap[1][pos] = 0;
			gmap::delsprite(spr);
		}
		else if (spr->id == "stairup1") {
			menus::dialogue("the way is\nblocked.");
		}
		else if (spr->id == "stairdown1") {
			menus::dialogue("the way is\nblocked.");
		}
		else
			return 0;
		return 1;
	}

} // end mygame
