// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// Hud

void hud_paint (void) {
	nt = NAMETABLE_B;
	if (pselected == 0 || game_mode != GM_SINGLE)
		p_s (1, 1, "ESPINETE            DON PIMPON");
	else
		p_s (1, 1, "DON PIMPON            ESPINETE");

	p_s (1, 2, "#000000 %%% &    & %%% 000000#");

	if (game_mode == GM_SINGLE) p_s (18, 2, " PRESS START!");
	if (game_mode != GM_VS) p_s (13, 2, "  $   ");

	oam_spr (243, 21, 1, 0x20, 0);
}

void hud_paint_lives (void) {
	if (game_mode != GM_VS) {
		p_t1 (16, 2, aplives [0]);
	} else {
		if (apon [0]) p_t1 (14, 2, aplives [0]);
		if (apon [1]) p_t1 (17, 2, aplives [1]);
	}
}

void hud_update_life (void) {
	// If player 0, write space @ (9 + plife, 2);
	// If player 1, write space @ (22 - plife, 2);
	gp_addr = NAMETABLE_B + 64 + (pindex ? (22 - plife) : (9 + plife));
	*ul = MSB (gp_addr); ++ ul;
	*ul = LSB (gp_addr); ++ ul; 
	*ul = 0; ++ ul;
}

void hud_update_score (void) {
	gp_addr = NAMETABLE_B + 64 + (pindex ? 24 : 2); 
	*ul = MSB (gp_addr) | NT_UPD_HORZ; ++ ul;
	*ul = LSB (gp_addr); ++ ul; 
	*ul = 5; ++ ul;
	gp_gen = bcd_score + (pindex ? 5 : 0);
	bcd_print_ul ();
}
