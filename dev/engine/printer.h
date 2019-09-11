// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// printer.h - Printing functions

void fade_out (void) { 
	fader = 5; while (fader --) { 
		pal_bright (fader); ppu_wait_nmi (); 
		if (do_split) { scroll (256, 0); custom_split (); }
	} 
}
	
void fade_in (void) { 
	fader = 5; while (fader --) { 
		pal_bright (4 - fader); ppu_wait_nmi (); 
		if (do_split) { scroll (256, 0); custom_split (); }
	} 
}

void cls (void) { 
	vram_adr (NAMETABLE_A); vram_fill (0, 2048); 	
}

/*
void reset_attributes (void) {
	gpit = 64; while (gpit --) attr_table [gpit] = 0x55;
}

void show_attributes (void) {
	vram_adr (NAMETABLE_A + 0x03c0);
	vram_write (attr_table, 64);
}
*/

void _upd2h (void) {
	/*
	*ul ++ = MSB (gp_addr) | NT_UPD_HORZ;
	*ul ++ = LSB (gp_addr);
	*ul ++ = 2;	
	*/

		__asm__ ("ldy #$0");
		__asm__ ("lda %v+1", gp_addr);
		__asm__ ("ora #%b", NT_UPD_HORZ);
		__asm__ ("sta (%v), y", ul);
		
		__asm__ ("iny");
		__asm__ ("lda %v", gp_addr);
		__asm__ ("sta (%v), y", ul);
		
		__asm__ ("iny");
		__asm__ ("lda #2");
		__asm__ ("sta (%v), y", ul);

		__asm__ ("lda %v", ul);
		__asm__ ("clc");
		__asm__ ("adc #3");
		__asm__ ("sta %v", ul);
		__asm__ ("bcc %g", P_WGAH2);
		__asm__ ("inc %v+1", ul);
	P_WGAH2:
	;
}

void p_t2 (unsigned char x, unsigned char y, unsigned char n) {
	gp_addr = NAMETABLE_A + (y << 5) + x;
	_upd2h ();
	ul [0] = DIGIT ((n / 10));
	ul [1] = DIGIT ((n % 10));
	ul += 2;
}

void p_t2_d (unsigned char n) {
	vram_put (DIGIT (n/10));
	vram_put (DIGIT (n % 10));
}

void p_t1 (unsigned char x, unsigned char y, unsigned char n) {
	gp_addr = NAMETABLE_B + (y << 5) + x;
	ul [0] = MSB (gp_addr); 
	ul [1] = LSB (gp_addr);
	ul [2] = DIGIT (n);
	ul += 3;
}

void p_s (unsigned char x, unsigned char y, const unsigned char *s) {
	vram_adr (nt + (y << 5) + x);
#ifdef PRINT_PSEUDOASCII
	while (rdt = *s ++) vram_put (rdt - 32);
#endif
#ifdef PRINT_CUSTOM	
	while (0xff != (rdt = *s ++)) vram_put (rdt);
#endif
}

void ascii_to_ul (unsigned char *s) {
	rda = 0;
	while (rdb = *s ++) ul [rda ++] = rdb - 32;
	ul += rda;
}

void put_tile_ul (void) {
	// Draws rdt to rdx, rdy

	// The value in rdy is < 30 so, this important shourtcut
	// may be performed:
		__asm__ ("lda %v", rdy);
		__asm__ ("asl a");			// A = rdy * 2 (< 60)
		__asm__ ("asl a"); 			// A = rdy * 4 (< 120)
		__asm__ ("asl a"); 			// A = rdy * 8 (< 240)

	// Now we can extend to 16 bits and call aslax2.
		__asm__ ("ldx #0");
		__asm__ ("jsr aslax2");

	// Result is on X:A, now OR rdx to A:
		__asm__ ("ora %v", rdx);

	// And last but not least, add NAMETABLE_A = $2000
	// A = A + 0, X = X + 20, store in gp_addr.
		__asm__ ("sta %v", gp_addr);
		__asm__ ("txa");
		__asm__ ("clc");
		__asm__ ("adc #$20");
		__asm__ ("sta %v+1", gp_addr);

	// Which is way shorter that what cc65 produces.
	// Now write the address to ul, then the first row.

		_upd2h ();	

		__asm__ ("ldy %v", rdt);
		__asm__ ("lda (%v), y", c_ts_tmaps_0);
		
		__asm__ ("ldy #0");
		__asm__ ("sta (%v), y", ul);

		__asm__ ("ldy %v", rdt);
		__asm__ ("lda (%v), y", c_ts_tmaps_1);
		
		__asm__ ("ldy #1");
		__asm__ ("sta (%v), y", ul);

		__asm__ ("lda %v", ul);
		__asm__ ("clc");
		__asm__ ("adc #2");
		__asm__ ("sta %v", ul);
		__asm__ ("bcc %g", P_PTUS1);
		__asm__ ("inc %v+1", ul);
	P_PTUS1:

		__asm__ ("lda %v", gp_addr);
		__asm__ ("clc");
		__asm__ ("adc #32");
		__asm__ ("sta %v", gp_addr);
		__asm__ ("bcc %g", P_PTUS0);
		__asm__ ("inc %v+1", gp_addr);
	P_PTUS0:

		_upd2h ();

		__asm__ ("ldy %v", rdt);
		__asm__ ("lda (%v), y", c_ts_tmaps_0);
		
		__asm__ ("ldy #0");
		__asm__ ("sta (%v), y", ul);

		__asm__ ("ldy %v", rdt);
		__asm__ ("lda (%v), y", c_ts_tmaps_1);
		
		__asm__ ("ldy #1");
		__asm__ ("sta (%v), y", ul);

		__asm__ ("lda %v", ul);
		__asm__ ("clc");
		__asm__ ("adc #2");
		__asm__ ("sta %v", ul);
		__asm__ ("bcc %g", P_PTUS2);
		__asm__ ("inc %v+1", ul);
	P_PTUS2:
	;
}
