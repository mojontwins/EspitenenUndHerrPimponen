// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// Code for fixed screens

void enter_screen (const unsigned char *pal, void (*func) (void)) {
	scroll (0, 0);
	cls ();
	
	pal_bg (pal);
	(*func) ();
	
	ppu_on_all ();
	fade_in ();
}

void exit_cleanly (void) {
	oam_hide_rest (0);
	music_stop ();
	fade_out ();
	ppu_off ();	
}

void wait_time_or_input (void) {
	while (game_time) {
		ticker ++; if (ticker == ticks) {
			ticker = 0;
			game_time --;
		}

		get_pad_once ();
		if (pad_once || pad_doce) break;

		ppu_wait_nmi ();
	}
}

void do_screen (unsigned char seconds) {
	game_time = seconds; ticker = 0;
	wait_time_or_input ();
	exit_cleanly ();
}

void screen_print_stats (void) {
#ifdef LANG_ES
	p_s (8, rda,   "PUNTOS    "); gp_gen = bcd_score; bcd_print ();
	p_s (8, rda+1, "RECORD    "); gp_gen = bcd_high_score; bcd_print ();
#else	
	p_s (8, rda,   "SCORE     "); gp_gen = bcd_score; bcd_print ();
	p_s (8, rda+1, "HI SCORE  "); gp_gen = bcd_high_score; bcd_print ();
#endif	
}

void screen_logo (void) {
	nt = NAMETABLE_A;
	vram_adr (NAMETABLE_A); vram_unrle (title_png_rle);
	rda = 4; screen_print_stats ();
	p_s (5, 24, "@ 2019 THE MOJON TWINS");
}

void screen_title (void) {
	screen_logo ();

	p_s (10, 18, "1 PLAYER");
	p_s (10, 19, "2 PLAYER CO-OP");
	p_s (10, 20, "2 PLAYER VS.");
}

void screen_select (void) {
	screen_logo ();

	#ifdef LANG_ES
		p_s (8, 17, "ELIGE PERSONAJE:");
	#else
		p_s (8, 17, "SELECT CHARACTER");
	#endif

	p_s (11, 19, "ESPINETE");
	p_s (11, 20, "DON PIMPON");
}

void screen_select_level (void) {
	screen_logo ();

	#ifdef LANG_ES 
		p_s (10, 17, "ELIGE NIVEL:");
		p_s (11, 20, "AL AZAR!");
	#else		
		p_s (10, 17, "SELECT LEVEL");
		p_s (11, 20, "RANDOM!");
	#endif

	p_s (11, 19, "^          _");
}

void screen_game_over (void) {
	nt = NAMETABLE_A;
	// Game over
	if (level && pcontinues) {
		rda = 11;
#ifdef LANG_ES		
		p_s (11, 15, "CONTINUA "); vram_put (DIGIT (pcontinues));
		p_s (12, 16, "SI^  _NO");
#else	
		p_s (11, 15, "CONTINUE "); vram_put (DIGIT (pcontinues));
		p_s (12, 16, "YES / NO");
#endif		
	} else rda = 13;
	p_s (11, rda, "GAME OVER!");
	rda += 2; screen_print_stats ();
}

void screen_ending (void) {
	nt = NAMETABLE_A;
	vram_adr (NAMETABLE_A); vram_unrle (pselected ? cuts1_png_rle : cuts0_png_rle);
#ifdef LANG_ES
	if (pselected) {
		p_s (3, 16, "'[HOLA, DON PIMPON! [QUE");
		p_s (3, 18, "SORPRESA ENCONTRARTE AQUI!'");
		p_s (3, 20, "'<QUIEN SOY? <DONDE ESTOY?");
		p_s (3, 22, "<QUIEN ES USTED, SE]ORITA?'");
	} else {
		p_s (4, 16, "'EH, TE GANO OTRA VEZ AL");
		p_s (4, 18, "ESCONDITE, ESPINETE.'");
		p_s (4, 20, "'[JO, ANA, ES QUE TE VAS");
		p_s (4, 22, "MUY LEJOS! PS!'");
	}
#else
	if (pselected) {
		p_s (4, 16, "'HELLO, DON PIMPON! WHAT");
		p_s (4, 18, "A SURPRISE!'");
		p_s (4, 20, "'WHO AM I? WHERE AM I?");
		p_s (4, 22, "WHO ARE YOU, MISS?'");
	} else {
		p_s (4, 16, "'I WON AGAIN OUR GAME OF'");
		p_s (4, 18, "HIDE AND SEEK ESPINETE.'");
		p_s (4, 20, "'DARN, ANA, YOU HID WAY");
		p_s (4, 22, "TOO FAR AWAY! PS!'");	
	}
#endif	
}

void screen_score (void) {
	#ifdef LANG_ES
		p_s (9, 19, "RONDAS GANADAS");
	#else
		p_s (11, 19, "ROUNDS WON");		
	#endif
		
	p_s (10, 20, "PLAYER 1: "); p_t2_d (pscore [0]);
	p_s (10, 21, "PLAYER 2: "); p_t2_d (pscore [1]);
}

void screen_round_results (void) { 
	screen_logo ();

	#ifdef LANG_ES
		if (winner < 2) {
			p_s (5, 16, "PLAYER ");
			vram_put (17 + winner);
			p_s (14, 16, "GANA RONDA ");
			vram_put (17 + round);
		} else {
			p_s (11, 16, "UN EMPATE!");
		}
	#else
		if (winner < 2) {
			p_s (5, 16, "PLAYER ");
			vram_put (17 + winner);
			p_s (14, 16, "WINS ROUND ");
			vram_put (17 + round);
		} else {
			p_s (11, 16, "DRAW GAME!");
		}
	#endif	

	screen_score ();
}

void screen_game_result (void) {
	screen_logo ();
	screen_score ();

	p_s (9, 16, "PLAYER ");
	vram_put (17 + winner);
	
	#ifdef LANG_ES
		p_s (18, 16, "GANA!");
	#else
		p_s (18, 16, "WINS!");
	#endif
}
