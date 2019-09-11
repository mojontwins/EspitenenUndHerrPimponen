// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// エスピネテ 

#define STANDALONE
#define CNROM
//#define ROLLED_SCROLLER
//#define DEBUG

extern unsigned char *ATTRIB_READ;
extern unsigned char *SCROLL_WRITE;
extern unsigned char *ATTRIB_WRITE;
#pragma zpsym ("ATTRIB_READ")
#pragma zpsym ("SCROLL_WRITE")
#pragma zpsym ("ATTRIB_WRITE")

#include "neslib-pantanow-CNROM.h"

// Definitions

#include "definitions.h"

// Assets
#include "assets/map0.h"
#include "assets/enems0.h"
#include "assets/map1.h"
#include "assets/enems1.h"
#include "assets/map2.h"
#include "assets/enems2.h"
#include "assets/map3.h"
#include "assets/enems3.h"
#include "assets/map4.h"
#include "assets/enems4.h"
#include "assets/map5.h"
#include "assets/enems5.h"
#include "assets/map6.h"
#include "assets/enems6.h"
#include "assets/map8.h"
#include "assets/enems8.h"
#include "assets/map9.h"
#include "assets/enems9.h"
#include "assets/palettes.h"
#include "assets/tiledata.h"
#include "assets/spritedata.h"
#include "assets/spriteset.h"
#include "assets/precalcs.h"
#include "assets/behs.h"
#include "assets/levelset.h"
#include "assets/level_names.h"
#include "assets/title_rle.h"
#include "assets/cuts0_rle.h"
#include "assets/cuts1_rle.h"

// Some variables

#pragma bss-name (push,"ZEROPAGE")
#pragma data-name(push,"ZEROPAGE")

#include "ram/zp.h"

#pragma bss-name (push,"BSS")
#pragma data-name(push,"BSS")

#include "ram/bss.h"

// Engine functions

#include "engine/mybcd.h"
#include "engine/general.h"
#include "engine/custom_split.h"
#include "engine/printer.h"
#include "engine/scroller_rle.h"
#include "engine/gbuffer.h"
#include "engine/hud.h"
#include "engine/puas_assembly.h"
#ifdef POINTS_ENABLE
#include "engine/points_assembly.h"
#endif
#include "engine/player.h"
#ifndef ENEM_DISABLE_BULLETS
#include "engine/bullets.h"
#endif
#include "engine/enems.h"
#include "engine/screens.h"
#include "engine/game.h"

void main (void) {
#ifndef STANDALONE	
	m11_handle_reset ();		// If bad checksum, this jumps to PRG 0 / CHR 0 (main menu)
#endif	
	
	ntsc = ppu_system ();
	ticks = ntsc ? 60 : 50;
	halfticks = ticks >> 1;
	do_split = level_selected = pselected = 0;

	bank_bg (0);
	bank_spr (1);
	pal_spr (palss0);

	bcd_score_init ();
	bcd_high_score_init ();

	pal_bright (0);

	while (1) {
		bankswitch (3);
		if (signal_continue == 0) {
			game_title ();

			if (game_mode == GM_SINGLE) {
				game_select ();
				apcharacter [0] = pselected;
				apcharacter [1] = 1 - pselected;				
			} else {
				apcharacter [0] = 0;
				apcharacter [1] = 1;
			}

			level = 0;
			pcontinues = 3;
		} else pcontinues --;

		bcd_score_init ();

		if (game_mode == GM_VS) {
			aplives [0] = aplives [1] = round = 0;
			 // Select level / random level
 			game_select_level ();
		} else {
			aplives [0] = aplives [1] = PLAYER_LIVES_INI;
		}
		
		collide_players = (game_mode == GM_VS);

		while (1) {
 			apon [0] = 1;			
 			if (game_mode != GM_SINGLE) {
 				apon [1] = 1;
 			} 

 			if (game_mode == GM_VS) {
 				bcd_score_init ();
 			}

			game_init ();
			game_loop ();

			if (game_mode == GM_VS) {
				// Update results
				winner = bcd_compare_scores ();
				if (winner < 2) ++ pscore [winner];

				// Show results
				game_round_results ();

				// Decide who wins or continue game
				if (pscore [0] > pscore [1] + 1) {
					winner = 0;
				} else if (pscore [1] > pscore [0] + 1) {
					winner = 1;
				} else winner = 2;

				if (winner < 2) {
					game_game_result ();
					signal_continue = 0;
					break;
				} else round ++;
			} else {
				if (pkill) {
					if (aplives [0]) {
						aplives [0] --;
					} else {
						bcd_high_score_update ();
						game_over ();
						break;
					}
				} else {
					level ++;
					if (level == 10) {
						game_ending ();
						level = 0;
					}
				}
			}
		}

		// Zero score after a 2 Players game
		if (game_mode != GM_SINGLE) bcd_score_init ();
	}
}
