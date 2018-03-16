#include "globals.h"
#include <sstream>
using namespace std;

namespace battle {
	// define
	enum Elements {
		EL_PHYSICAL=0,
		EL_FLAME
	};
	struct Spell {
		std::string name;
		int lvl, dmg, mp, elem;
	};
	// lists
//	const vector<int> levelxp = { 0, 5 };
	const vector<Spell> spellslist = {
		{ "flame", 1, 1, 2, EL_FLAME }
	};
	BattleStats player={"player",1,1,3,EL_PHYSICAL}, enemy={"blkslime",1,1,1,EL_PHYSICAL};
	int plxp = 0;
	
	void rest(BattleStats& st) {
		st.hp = st.stm * 5;
		st.mp = st.intl * 5;
	}
	
	void statsbox(int x, int y) {
		const BattleStats& st = player;  // always show player for now
		stringstream ss;
		ss <<st.hp<<"/"<<st.stm*5<<"\n";
		ss <<"mp  "<<st.mp<<"\n";
		ss <<"str "<<st.str<<"\n";
		ss <<"stm "<<st.stm<<"\n";
		ss <<"int "<<st.intl<<"\n";
		menus::txtbox( { int16_t(x), int16_t(y), 50, 46 }, ss.str() );
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
			statsbox(100, 90);
			// main menu
			auto opt = menus::showlist({ 10, 90, 60, 4+8*3 }, {"fight", "magic", "flee"});
			if (opt == "flee")  break;
			else if (opt == "fight") {
				exchange(player.str, 0);
			}
			else if (opt == "magic") {
				// setup
				int plevel = player.str + player.intl + player.stm;
				vector<string> magiclist;
				for (const auto& sp : spellslist)
					if (plevel >= sp.lvl)  magiclist.push_back(sp.name);
				magiclist.push_back("back");
				// 
				while (true) {
					Spell spell = {"none"};
					auto opt = menus::showlist({ 50, 90, 60, 4+8*4 }, magiclist);
					if (opt == "back")  break;
					else if (opt == "flame")  spell = spellslist[0];
					// do cast
					if (player.mp >= spell.mp) {
						exchange(player.intl+1, spell.elem);
						player.mp -= spell.mp;
						break;
					}
					else
						menus::dialogue("not enough mana.");
				}
			}
			//
			if (player.hp == 0 || enemy.hp == 0)  break;
		}
		if (enemy.hp == 0) {
			int xp = enemy.str + enemy.intl + enemy.stm;
			stringstream ss;
			ss << "you gain "<<xp<<" xp";
			menus::dialogue(ss.str());
			plxp += xp;
		}
		if (player.hp == 0) {
			menus::dialogue("you die.");
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
