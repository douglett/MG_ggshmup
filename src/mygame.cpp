#include "globals.h"
using namespace std;

namespace mygame {

	int init() {
		return 0;
	}
	
	int action(const std::string& mapname, gmap::Sprite* spr) {
		if (spr == NULL)  
			return 0;
		else if (spr->id == "test2") {
			menus::dialogue("test 123 hello\nworld!");
			paint1();
			battle::begin();
			if (battle::player.hp == 0)  exit(0);
			if (battle::enemy.hp == 0)  gmap::delsprite(spr);
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
