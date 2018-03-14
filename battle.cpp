#include "globals.h"
using namespace std;

namespace battle {

	struct BattleStats {
		std::string name;
		int str, intl, stm;
		int lvl, elem;
		int hp, mp;
	};
	
	BattleStats player={"player",1,1,4,6}, enemy={"blkslime",1,1,2,4};
	
	void exchange(int pdmg, int pelem);
	
	void rest(BattleStats& st) {
		st.hp = st.stm * 5;
		st.mp = st.intl * 5;
	}
	
	void begin() {
		// init
		rest(player);
		rest(enemy);
		SDL_Surface* sf = clonesurface(buf);
		// main loop
		while (true) {
			// repaint
			SDL_BlitSurface(sf, NULL, buf, NULL);
			// enemy bg
			SDL_Rect dst = { 30, 20, 100, 60 };
			SDL_FillRect(buf, &dst, 0xffffffff);
			dst.x++, dst.y++, dst.w-=2, dst.h-=2;
			SDL_FillRect(buf, &dst, 0x000000ff);
			// main menu
			auto opt = menus::showlist({ 10, 90, 60, 4+8*3 }, {"fight", "magic", "flee"});
			if (opt == "flee")  break;
			else if (opt == "fight") {
				exchange(player.str, 0);
			}
			else if (opt == "magic") {
				while (true) {
					auto opt = menus::showlist({ 50, 90, 60, 4+8*4 }, {"flame", "back"});
					if (opt == "back")  break;
					else if (opt == "flame") {
						exchange(player.intl, 0);
						break;
					}
				}
			}
			//
			if (player.hp == 0 || enemy.hp == 0)  break;
		}
		// deinit
		SDL_FreeSurface(sf);
	}
	
	void exchange(int pdmg, int pelem) {
		menus::dialogue("you attack for\n"+to_string(pdmg)+" damage!");
		enemy.hp = max(0, enemy.hp - pdmg);
		if (enemy.hp == 0) {
			menus::dialogue(enemy.name.substr(0,9) + " dies!");
			return;
		}
		menus::dialogue(enemy.name.substr(0,9)+" attacks\nfor "+to_string(enemy.str)+" damage!");
		player.hp = max(0, player.hp - enemy.str);
		if (player.hp == 0) {
			menus::dialogue("you die.\nyou suck.");
			return;
		}
	}
	
} // end battle
