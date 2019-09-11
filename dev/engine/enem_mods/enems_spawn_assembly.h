// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

void enems_spawn (void) {
		// strip_pointer is a pointer in ZP, so we can...
		__asm__ ("lda #0");
		__asm__ ("sta %v+1", rdy);
		__asm__ ("ldy #0");
		__asm__ ("lda (%v), y", strip_pointer);
		__asm__ ("sta %v", rdy);
		
		// A 0 in the stream means no more stuff 
		__asm__ ("bne %g", enems_spawn_check);
		__asm__ ("rts");


	enems_spawn_check:
		// Check if the scroll has reached that point
		__asm__ ("cmp %v", map_tilepos);
		__asm__ ("beq %g", enems_spawn_do);
		__asm__ ("rts");

	enems_spawn_do:
		// Read two more bytes
		__asm__ ("iny");
		__asm__ ("lda (%v), y", strip_pointer);
		__asm__ ("sta %v", rdx);
		__asm__ ("iny");
		__asm__ ("lda (%v), y", strip_pointer);
		__asm__ ("sta %v", rdc);

		// rdx == XXXXTTTT rdc == BBBBAAAA

		// Find a free slot.
		__asm__ ("lda %v", ensloti);
		__asm__ ("beq %g", enems_spawn_done);

		__asm__ ("dec %v", ensloti);
		__asm__ ("ldy %v", ensloti);
		__asm__ ("lda %v, y", enslots);
		__asm__ ("sta %v", enit);
		__asm__ ("tay");

		// Enemy active:
		__asm__ ("lda #1");
		__asm__ ("sta %v, y", ena);

		// Enemy type:
		__asm__ ("lda %v", rdx);
		__asm__ ("and #$0f");
		__asm__ ("sta %v, y", ent);

		// Enemy X
		__asm__ ("lda %v", rdx);
		__asm__ ("and #$f0");
		__asm__ ("sta %v, y", enx);

		// Enemy Y is 16 bit
		__asm__ ("tya");
		__asm__ ("asl a");
		__asm__ ("tay");				// Index*2

		__asm__ ("lda %v", rdy);		// LSB in A
		__asm__ ("ldx %v+1", rdy);		// MSB in X
		__asm__ ("jsr aslax4");			// X:A =<< 4
		
		__asm__ ("sta %v, y", eny);
		__asm__ ("sta %v, y", eny1);
		__asm__ ("txa");
		__asm__ ("sta %v+1, y", eny);
		__asm__ ("sta %v+1, y", eny1);

		// Restore index:
		__asm__ ("ldy %v", enit);

		// enl
		__asm__ ("lda #0");
		__asm__ ("sta %v, y", enl);

		// Parameter A, parameter B from rdc:
		__asm__ ("lda %v", rdc);
		__asm__ ("and #$0f");
		__asm__ ("sta %v", rda);

		__asm__ ("lda %v", rdc);
		__asm__ ("and #$f0");
		__asm__ ("sta %v", rdb);

		// Now set up more stuff depending on type:

		__asm__ ("lda %v, y", ent);

		//__asm__ ("cmp #2");
		//__asm__ ("beq %g", enems_spawn_bf);

		//__asm__ ("cmp #3");
		//__asm__ ("beq %g", enems_spawn_fallers);

		// Enems 2, 3 are default

		__asm__ ("cmp #4");
		__asm__ ("beq %g", enems_spawn_chasers);

		__asm__ ("cmp #6");
		__asm__ ("beq %g", enems_spawn_flipflop);

		__asm__ ("cmp #7");
		__asm__ ("beq %g", enems_spawn_platforms);

	//enems_spawn_bf:
	//enems_spawn_fallers:

		// Arcane I no longer understand
		__asm__ ("lda %v", rda);
		__asm__ ("bne %g", ESBF0);

		__asm__ ("sta %v, y", enx1);
		__asm__ ("lda #1");
		__asm__ ("sta %v", rda);
		__asm__ ("jmp %g", ESBF1);
	ESBF0:
		__asm__ ("lda #1");
		__asm__ ("sta %v, y", enx1);
	ESBF1:

		// mx
		__asm__ ("lda %v", rdb);
		__asm__ ("and #$10");
		__asm__ ("beq %g", ESBF2);

		__asm__ ("lda #0");
		__asm__ ("sec");
		__asm__ ("sbc %v", rda);
		__asm__ ("jmp %g", ESBF3);		// A = -rda
	ESBF2:
		__asm__ ("lda %v", rda);		// A = rda
	ESBF3:
		__asm__ ("sta %v, y", enmx);

		// my
		__asm__ ("lda #0");
		__asm__ ("sta %v, y", enmy);

		__asm__ ("jmp %g", enems_spawn_done);

	enems_spawn_chasers:
		__asm__ ("lda #0");

		__asm__ ("sta %v, y", enmx);
		__asm__ ("sta %v, y", enmy);

		__asm__ ("jmp %g", enems_spawn_done);

	enems_spawn_flipflop:
		__asm__ ("lda #0");
		__asm__ ("sta %v, y", ENSTATE);

		__asm__ ("lda %v, y", enx);
		__asm__ ("and #$7f");
		__asm__ ("sta %v, y", enct);

		__asm__ ("jmp %g", enems_spawn_done);

	enems_spawn_platforms:
		__asm__ ("lda %v", rdb);
		__asm__ ("sta %v, y", enx2);

		__asm__ ("lda %v", rda);
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("sta %v, y", enx1);

		__asm__ ("cmp %v", rdb); 	// A = enx1, cmp enx2
		__asm__ ("bcs %g", ESPSWP);

		__asm__ ("lda #1");
		__asm__ ("sta %v, y", enmx);
		__asm__ ("jmp %g", ESPSWPD);

	ESPSWP:
		__asm__ ("lda #%b", -1);
		__asm__ ("sta %v, y", enmx);
		
		// Swap x1 and x2
		__asm__ ("lda %v, y", enx1);
		__asm__ ("sta %v", rdd);
		__asm__ ("lda %v, y", enx2);
		__asm__ ("sta %v, y", enx1);
		__asm__ ("lda %v", rdd);
		__asm__ ("sta %v, y", enx2);
		__asm__ ("sta %v, y", enx);

	ESPSWPD:

		__asm__ ("jmp %g", enems_spawn_done);

	enems_spawn_done:
		// Advance pointer
		__asm__ ("lda %v", strip_pointer);
		__asm__ ("clc");
		__asm__ ("adc #%b", ENEM_SIZE);
		__asm__ ("sta %v", strip_pointer);
		__asm__ ("bcc %g", ESAPS0);
		__asm__ ("inc %v+1", strip_pointer);
	ESAPS0:
	;
}
