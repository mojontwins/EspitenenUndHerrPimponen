// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// Game loops & fixed screens

void game_init (void) {
	scroll_init ();

	map_ptr = level_map [level];
	behs = level_behs [level];
	strip_pointer = level_strips [level];
	c_ts_tmaps_0 = level_ts_tmaps_0 [level];
	c_ts_tmaps_1 = level_ts_tmaps_1 [level];
	c_ts_tmaps_2 = level_ts_tmaps_2 [level]; 
	c_ts_tmaps_3 = level_ts_tmaps_3 [level]; 
	c_ts_pals_0 = level_ts_pals_0 [level];
	c_ts_pals_1 = level_ts_pals_1 [level];
	c_ts_pals_2 = level_ts_pals_2 [level];
	c_ts_pals_3 = level_ts_pals_3 [level];
	c_spr_base = level_spr_base [level];

	// Bitfield to define which enemy types are killable
	c_level_killable_enems = level_killable_enems [level];

	// Scrolling speed
	scroll_speed = 129;

	pal_bg  (level_pal_bg [level]);
	//pal_spr (level_pal_fg [level]);

	spr_enems = level_spr_enems [level];

	players_init ();
	enems_init ();

	#ifndef ENEM_DISABLE_BULLETS
		bullets_init ();
	#endif

	#ifdef PLAYER_PUAS
		puas_init ();
	#endif

	#ifdef POINTS_ENABLE
		points_init ();
	#endif

	switch (level) {
		case 4:
		case 8:
			pal_cycle [0] = 0x17;
			pal_cycle [1] = 0x18;
			pal_cycle [2] = 0x27;
			break;
		case 6:
			pal_cycle [0] = 0x0f;
			pal_cycle [1] = 0x0f;
			pal_cycle [2] = 0x2c;
			break;
	}
	pal_cycle_do = (level == 4 || level == 6 || level == 8);
}

void game_loop (void) {
	nt = NAMETABLE_A;

	cls ();

	if (game_mode == GM_VS) {
		#ifdef LANG_ES
			p_s (12, 14, "RONDA: ");
		#else
			p_s (12, 14, "ROUND: ");
		#endif
		vram_put (17 + round);
	} else {
		if (level < 9) {
			#ifdef LANG_ES
				p_s (13, 14, "FASE ");
			#else
				p_s (12, 14, "STAGE: ");
			#endif
			vram_put (17 + level);
		} else {
			#ifdef LANG_ES
				p_s (11, 14, "FASE FINAL");
			#else
				p_s (11, 14, "LAST STAGE");
			#endif
		}
	}
	
	hud_paint ();
	pal_bright (0);
	ppu_on_all ();

	set_vram_update (update_list);

	autoscroll = ntsc ? 112 : 120;
	goal_time = 120;

	scroll (256, 0);
	
	ul = update_list;
	if (apon [0]) { pindex = 0; hud_update_score (); }
	if (apon [1]) { pindex = 1; hud_update_score (); }
	hud_paint_lives ();
	*ul = NT_UPD_EOF;
	ppu_wait_nmi ();

	do_split = 1;
	fade_in ();

	fskipct = is_scrolling = 0;
	signal10000 = 0;

	#ifdef DEBUG
		debug_on = 0;
	#endif

	music_play (MUSIC_INGAME + (level & 3));
	
	//delay (ticks); Needs to split
	gpit = ticks; while (gpit) { --gpit; 
		ppu_wait_nmi (); scroll (256, 0); custom_split ();
	}

	while (1) {
		ul = update_list;
		sadv = 0;

		if (ntsc && fskipct ++ == 6) {
			fskipct = 0;
		} else {
			half_life = 1 - half_life;
			hl_proc = half_life;
			frame_counter ++;
			ticker ++; if (ticker == 50) ticker = 0;
			tick = (ticker == 0);
			
			//pad0 = pad_poll (0);
			get_pad_once ();
			free_frame = 0;

			if (autoscroll) {
				autoscroll --; sadv = 2;
			} else if ((apkill [0] == 0 || apon [0] == 0) && (apkill [1] == 0 || apon [1] == 0)) {
				if (is_scrolling) {
					sadv = half_life;
					if (cam_pos <= scroll_to) is_scrolling = 0;
				} else {
					if (ppossee || pgotten || psinking) {
						if (cam_pos > 0) {
							if (
								(apon [0] && apry [0] < cam_pos + 104)
								||
								(apon [1] && apry [1] < cam_pos + 104)
							) {
								is_scrolling = 1;
								scroll_to = 0; // Once you pop, never stop!														
							}
						}
					}
				}
			}
			scroll_advance (sadv);

			oam_index = 4;

			enems_spawn ();

			#ifdef POINTS_ENABLE
				points_do ();
			#endif
			
			players_move ();

			enems_do ();

			#ifndef ENEM_DISABLE_BULLETS
				bullets_do ();
			#endif

			#ifdef PLAYER_PUAS
				puas_do ();
			#endif


			if (signal10000) {
				if (aplives [0] < 9) aplives [0] ++;
				hud_paint_lives ();
				sfx_play (SFX_FANFARE, SC_MAIN);
				signal10000 = 0;
			}

			oam_hide_rest (oam_index);
		}

		if (pal_cycle_do && (frame_counter & 7) == 0) {
			rda = pal_cycle [2];
			pal_cycle [2] = pal_cycle [1]; pal_col (7, pal_cycle [1]);
			pal_cycle [1] = pal_cycle [0]; pal_col (6, pal_cycle [0]);
			pal_cycle [0] = rda;           pal_col (5, rda);
		}

		// Join in
		if (game_mode == GM_SINGLE) {
			if (pad1 & PAD_START) {
				game_mode = GM_COOP;
				pindex = 1; player_init ();
				player_find_safe_spot ();
				
				// NAMETABLE_B + 18 + 32*2 = $2452
				ul [0] = 0x24 | NT_UPD_HORZ;
				ul [1] = 0x52;
				ul [2] = 13;
				ul += 3;
				ascii_to_ul ("  %%% 000000#");
			}
		}

		// VS Respawn
		if (game_mode == GM_VS) {
			if (apon [0] == 0 && aplife [0]) {
				pindex = 0; player_respawn ();
			}
			if (apon [1] == 0 && aplife [1]) {
				pindex = 1; player_respawn ();
			}
		}

		*ul = NT_UPD_EOF;
		ppu_wait_nmi ();
		scroll (256, 0);
		scroll_paint ();
		custom_split ();		

		#ifdef CNROM
			// BG animation
			bankswitch (bg_anim [(frame_counter >> 3) & 3]);
		#endif		
		
		#ifdef PDEAD_UPRISE
			if (pkill && pry < cam_pos) break;
		#else
			// Kill player?
			rdu16 = 224 + cam_pos;
			if (game_mode == GM_SINGLE)	{
				if (apkill [0] && apry [0] > rdu16) break;
			} else {
				if (apkill [0] && apry [0] > rdu16) apon [0] = 0;
				if (apkill [1] && apry [1] > rdu16) apon [1] = 0;
				if (apon [0] == 0 && apon [1] == 0) {
					if (game_mode != GM_VS) break; else {
						if (aplife [0] == 0 && aplife [1] == 0) {
							break;
						}
					}
				}
			}
		#endif

		// Goal
		if (cam_pos == 0 && level < 9) goal_time --;
		if (goal_time == 0 || (pad0 & (PAD_UP|PAD_SELECT|PAD_B)) == (PAD_UP|PAD_SELECT|PAD_B)) break;		
	}

	music_stop ();

	fade_out ();	
	oam_hide_rest (0);
	set_vram_update (0);
	ppu_off ();

	set_scroll_write (0);
	do_split = 0;
	bankswitch (3);
}

void game_select_option (void) {
	while (1) {
		get_pad_once ();
		if (pad_once & (PAD_SELECT|PAD_DOWN)) { ++ rdb; if (rdb == rda) rdb = 0; }
		if (pad_once & PAD_UP) { if (rdb) -- rdb; else rdb = rda - 1;}
		if (pad_once & PAD_START) break;

		if (rdt) {
			// Level selection logic
			if (rdb == 0) {
				if (pad_once & PAD_LEFT) { if (level_selected) -- level_selected; else level_selected = 9; }
				if (pad_once & PAD_RIGHT) { ++ level_selected; if (level_selected == 10) level_selected = 0; }
			}

			update_list [0] = 0x22 | NT_UPD_HORZ;
			update_list [1] = 0x6c; 
			update_list [2] = 10;
			memcpy (update_list + 3, level_names + (level_selected << 3) + (level_selected << 1), 10);
			update_list [13] = NT_UPD_EOF;
		}

		oam_hide_rest (oam_spr (64, rdc + (rdb << 3), 0xf, 0, 0));
		ppu_wait_nmi ();
	}
}

void game_title (void) {
	enter_screen (paltitle, screen_title);
	rdb = game_mode; rda = 3; rdc = 143; rdt = 0;
	game_select_option ();
	game_mode = rdb;
	exit_cleanly ();
}

void game_select (void) {
	enter_screen (paltitle, screen_select);
	rdb = 0; rda = 2; rdc = 151; rdt = 0;
	game_select_option ();
	pselected = rdb;
	exit_cleanly ();
}

void game_over (void) {
	enter_screen (palts0, screen_game_over);
	signal_continue = 0;

	if (level && pcontinues) {
		while (1) {
			get_pad_once ();

			if (pad_once & PAD_RIGHT) {
				if (signal_continue) sfx_play (6, 0); signal_continue = 0;
			}

			if (pad_once & PAD_LEFT) {
				if (!signal_continue) sfx_play (6, 0); signal_continue = 1;
			}

			if (pad_once & (PAD_A|PAD_B|PAD_START)) { sfx_play (0, 0); break; }

			oam_hide_rest (oam_spr (signal_continue ? 88 : 160, 127, 0xf, 0, 0));
			ppu_wait_nmi ();
		}

		exit_cleanly ();
	} else do_screen (10);
}

void game_ending (void) {
	enter_screen (pselected ? palcuts1 : palcuts0, screen_ending);
	do_screen (240);
}

void game_round_results (void) {
	enter_screen (paltitle, screen_round_results);
	do_screen (255);
}

void game_game_result (void) {
	enter_screen (paltitle, screen_game_result);
	do_screen (255);
}

void game_select_level (void) {
	enter_screen (paltitle, screen_select_level);
	rdb = 0; rda = 2; rdc = 151; rdt = 1; rdd = level_selected;
	set_vram_update (update_list);	
	game_select_option ();
	set_vram_update (0);
	if (rdb == 0) level = level_selected; else level = rand8 () % 10;
	level_selected = rdd;
	exit_cleanly ();	
}
