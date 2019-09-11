// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// Beware! Y components are 16 bit!

void puas_init (void) {
		__asm__ ("ldx #%b", PUAS_MAX);
		__asm__ ("lda #0");

	puas_init_loop:
		__asm__ ("dex");
		__asm__ ("sta %v, x", puas_st);
		__asm__ ("txa");
		__asm__ ("bne %g", puas_init_loop);
}

void puas_create (void) {
		__asm__ ("ldx #%b", PUAS_MAX);		

	puas_create_loop:
		__asm__ ("dex");
		__asm__ ("lda %v, x", puas_st);
		__asm__ ("bne %g", puas_create_continue);
		
		__asm__ ("lda %v", prx);
		__asm__ ("sta %v, x", puas_x);

		__asm__ ("lda #1");
		__asm__ ("sta %v, x", puas_st);

		__asm__ ("lda %v", pfacing);
		__asm__ ("beq %g", puas_create_left);

		__asm__ ("lda #%b", PUAS_VX);
		__asm__ ("jmp %g", puas_create_facing);

	puas_create_left:
		__asm__ ("lda #%b", -PUAS_VX);

	puas_create_facing:
		__asm__ ("sta %v, x", puas_mx);

		__asm__ ("txa");
		__asm__ ("asl a");
		__asm__ ("tax");

		__asm__ ("lda %v", pry);
		__asm__ ("sta %v, x", puas_y);
		__asm__ ("lda %v+1", pry);
		__asm__ ("sta %v+1, x", puas_y);
		
		__asm__ ("rts");
		
	puas_create_continue:
		__asm__ ("txa");
		__asm__ ("bne %g", puas_create_loop);	
}

void puas_do (void) {
		__asm__ ("ldy #%b", PUAS_MAX);

	puas_do_loop:
		__asm__ ("dey");
		__asm__ ("tya");
		__asm__ ("asl a");
		__asm__ ("tax");

		__asm__ ("lda %v, y", puas_st);
		__asm__ ("beq %g", puas_do_continue);

		__asm__ ("lda %v, x", puas_y);
		__asm__ ("sta %v", _puas_y);
		__asm__ ("lda %v+1, x", puas_y);
		__asm__ ("sta %v+1", _puas_y);

		__asm__ ("lda %v, y", puas_mx);
		__asm__ ("sta %v", _puas_mx);

		__asm__ ("lda %v, y", puas_x);
		__asm__ ("sta %v", _puas_x);

		__asm__ ("lda %v, y", puas_st);
		__asm__ ("cmp #1");
		__asm__ ("bne %g", puas_do_state_2);
		
		__asm__ ("lda %v", _puas_x);
		__asm__ ("clc");
		__asm__ ("adc %v", _puas_mx);
		__asm__ ("sta %v", _puas_x);

		__asm__ ("jmp %g", puas_do_state_done);

	puas_do_state_2:
		__asm__ ("clc");
		__asm__ ("lda %v", _puas_y);		
		__asm__ ("adc #8");
		__asm__ ("sta %v", _puas_y);
		__asm__ ("bcc %g", PDS2NC);
		__asm__ ("inc %v+1", _puas_y);
	PDS2NC:

	puas_do_state_done:

		// Prune!
		// if (_puas_x < PUAS_VX ||
		__asm__ ("lda %v", _puas_x);
		__asm__ ("cmp #%b", PUAS_VX);
		__asm__ ("bcc %g", puas_do_prune_do);

		// _puas_x > 248 - PUAS_VX ||
		__asm__ ("cmp #%w", (248 - PUAS_VX + 1));
		__asm__ ("bcs %g", puas_do_prune_do);

		// _puas_y >= cam_pos + 232
		// Beware! 16 bit
		__asm__ ("clc");
		__asm__ ("lda %v", cam_pos);
		__asm__ ("adc #232");
		__asm__ ("sta %v", rdu16);
		__asm__ ("lda %v+1", cam_pos);
		__asm__ ("adc #0");
		__asm__ ("sta %v+1", rdu16);

		// _puas_y >= rdu16; branch if _puas_y < rdu16
		__asm__ ("lda %v", _puas_y);
		__asm__ ("cmp %v", rdu16);
		__asm__ ("lda %v+1", _puas_y);
		__asm__ ("sbc %v+1", rdu16);
		__asm__ ("bcc %g", puas_do_prune_else);

	puas_do_prune_do:
		__asm__ ("lda #0");
		__asm__ ("sta %v, y", puas_st);
		__asm__ ("jmp %g", puas_do_prune_done);

	puas_do_prune_else:
		__asm__ ("sty %v", gpit);
		if (_puas_y > cam_pos)	oam_index = oam_spr (
			_puas_x, _puas_y + SPRITE_ADJUST - cam_pos,
			PUAS_PATTERN,
			_puas_mx > 0 ? 0 : 0x40,
			oam_index
		);
		__asm__ ("lda %v", gpit);
		__asm__ ("tay");
		__asm__ ("asl a");
		__asm__ ("tax");

		__asm__ ("lda %v", _puas_x);
		__asm__ ("sta %v, y", puas_x);
		__asm__ ("lda %v", _puas_y);
		__asm__ ("sta %v, x", puas_y);
		__asm__ ("lda %v+1", _puas_y);
		__asm__ ("sta %v+1, x", puas_y);

	puas_do_prune_done:

	puas_do_continue:
		__asm__ ("tya");
		__asm__ ("bne %g", puas_do_loop);
}	
