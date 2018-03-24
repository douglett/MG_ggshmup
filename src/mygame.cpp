#include "globals.h"
using namespace std;

namespace mygame {

	int init() {
		// castle map
		gmap::loadascii({
			"############",
			"#..##..##..#",  // 30
			"#..........#",
			"#..........#",
			"#####DD#####",
			"T,,,,,,,,,,T",
			"T,,,,,,,,,,T",
			"##D##TTTTTTT",
			"#...#TTTTTTT",
			"#....,,,,,,T",
			"#####T,,,,,T",
			"TTTTT,,,,,,T",  // 20
			"T,,,,,,,,,,T",
			"T,,,,,,,,,,T",
			"T,,TT,,TT,TT",
			"TTTTT,,TTTTT",
			"TTTTT,TTTTTT",
			"TTTTT,TTTTTT",
			"TTTTT,TTTTTT",
			"TTTTT,,TTTTT",
			"TTTTT,,TTTTT",
			"T,,TT,,TT,TT",  // 10
			"T,,,,,,,,,,T",
			"T,,,,,,,,,,T",
			"T,,,##,,,,,T",
			"#####DD#####",
			"#.....##...#",
			"#..........#",
			"#####..#####",
			"#/..##.#..|#",
			"#..........#",  // 1
			"############",  // 0
		});
		// set npc sprites
		etc::SrcImg guy1 = {{16, 18*2, 16, 18}, guy}, nil1 = {{0}, NULL};
		int ym = gmap::height-1;
		gmap::spritelist.push_back({ "guy",      {5 * 16, int16_t((ym - 1)*16),  16, 16},  guy1 });
		gmap::spritelist.push_back({ "secret1",  {1 * 16, int16_t((ym - 5)*16),  16, 16},  nil1 });
		gmap::spritelist.push_back({ "enemy1",   {6 * 16, int16_t((ym - 2)*16),  16, 16},  guy1 });
		gmap::spritelist.push_back({ "enemy1",   {5 * 16, int16_t((ym - 5)*16),  16, 16},  guy1 });
		gmap::spritelist.push_back({ "secret1",  {1 * 16, int16_t((ym - 10)*16), 16, 16},  nil1 });
		gmap::spritelist.push_back({ "enemy2",   {5 * 16, int16_t((ym - 14)*16), 16, 16},  guy1 });
		gmap::spritelist.push_back({ "secret2",  {6 * 16, int16_t((ym - 16)*16), 16, 16},  nil1 });
		gmap::spritelist.push_back({ "secret2",  {1 * 16, int16_t((ym - 17)*16), 16, 16},  nil1 });
		gmap::spritelist.push_back({ "secret1",  {10 *16, int16_t((ym - 22)*16), 16, 16},  nil1 });
		gmap::spritelist.push_back({ "enemy3",   {2 * 16, int16_t((ym - 23)*16), 16, 16},  guy1 });
		gmap::spritelist.push_back({ "secret1",  {1 * 16, int16_t((ym - 30)*16), 16, 16},  nil1 });
		gmap::spritelist.push_back({ "secret2",  {10 *16, int16_t((ym - 30)*16), 16, 16},  nil1 });
		gmap::spritelist.push_back({ "enemy4",   {6 * 16, int16_t((ym - 30)*16), 16, 16},  guy1 });
		//
		battle::rest(battle::player);
		return 0;
	}
	
	int action(const std::string& mapname, gmap::Sprite* spr) {
		if (spr == NULL)  
			return 0;
			
		// special
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
		
		// enemies
		else if (spr->id == "enemy1") {
			menus::dialogue("Halt! Return to\nyour cell or die!");
			paint1();
			battle::begin();
			if (battle::player.hp == 0)  exit(0);
			if (battle::enemy.hp  == 0)  gmap::delsprite(spr);
		}
		else if (spr->id == "enemy2") {
			menus::dialogue("What are you\ndoing in my\nforest?");
			paint1();
			battle::begin();
			if (battle::player.hp == 0)  exit(0);
			if (battle::enemy.hp  == 0)  gmap::delsprite(spr);
		}
		else if (spr->id == "enemy3") {
			menus::dialogue("Where is my\nbrother, the\nwoodcutter?");
			menus::dialogue("I will avenge\nhim!");
			paint1();
			battle::begin();
			if (battle::player.hp == 0)  exit(0);
			if (battle::enemy.hp  == 0)  gmap::delsprite(spr);
		}
		else if (spr->id == "enemy4") {
			menus::dialogue("I am the evil\nking!");
			menus::dialogue("It was I who\ncaptured you and\nate your child!");
			menus::dialogue("Prepare to be\neaten!");
			paint1();
			battle::begin();
			if (battle::player.hp == 0)  exit(0);
			if (battle::enemy.hp  == 0)  gmap::delsprite(spr);
		}
		
		// secrets
		else if (spr->id == "secret1") {
			string item = "h-potion";
			menus::dialogue("You found a\n"+item+"!");
			menus::items.push_back(item);
			gmap::delsprite(spr);
		}
		else if (spr->id == "secret2") {
			string item = "m-potion";
			menus::dialogue("You found a\n"+item+"!");
			menus::items.push_back(item);
			gmap::delsprite(spr);
		}
		
		// end
		else
			return 0;
		return 1;
	}

} // end mygame
