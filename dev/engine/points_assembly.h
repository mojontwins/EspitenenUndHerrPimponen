// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// Points, displays a small sprite going upwards for 16 frames.

// Assembly version

void points_init (void) {
		__asm__ ("ldx #%b", POINTS_MAX);
		__asm__ ("lda #0");
	points_init_loop:
		__asm__ ("sta %v,x", points_fc);
		__asm__ ("dex");
		__asm__ ("bne %g", points_init_loop);

		__asm__ ("sta %v", points_slots_index);
}

void points_create (void) {
		__asm__ ("lda %v", points_slots_index);
		__asm__ ("clc");
		__asm__ ("adc #1");
		__asm__ ("and #3");
		__asm__ ("sta %v", points_slots_index);

		__asm__ ("tay");

		__asm__ ("lda %v", rdx);
		__asm__ ("sta %v, y", points_x);

		__asm__ ("lda %v", rdy);
		__asm__ ("sta %v, y", points_y);

		__asm__ ("lda #24");
		__asm__ ("sta %v, y", points_fc);

		__asm__ ("lda %v", rdt);
		__asm__ ("sta %v, y", points_id);
}

void points_do (void) {
		__asm__ ("ldx #%b", POINTS_MAX);
		__asm__ ("stx %v", gpit);
	points_do_loop:
		__asm__ ("ldx %v", gpit);	
		__asm__ ("bne %g", points_do_loop_do);
		__asm__ ("rts");

	points_do_loop_do:
		__asm__ ("dex");
		__asm__ ("stx %v", gpit);

		__asm__ ("lda %v, x", points_fc);
		__asm__ ("bne %g", points_do_loop_do_do);
		__asm__ ("jmp %g", points_do_loop);

	points_do_loop_do_do:
		__asm__ ("dec %v, x", points_fc);

		__asm__ ("lda %v", frame_counter);
		__asm__ ("and #7");
		__asm__ ("bne %g", points_do_loop_move_skip);

		__asm__ ("dec %v, x", points_y);
	points_do_loop_move_skip:

		oam_index = oam_meta_spr (
			points_x [gpit], points_y [gpit],
			oam_index,
			spr_points [points_id [gpit]]
		);

		__asm__ ("jmp %g", points_do_loop);
}
