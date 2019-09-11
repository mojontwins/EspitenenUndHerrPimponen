// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

void bcd_score_init (void) {
	memfill (bcd_score, 16, 10);
}

void bcd_high_score_init (void) {
	memfill (bcd_high_score, 16, 5);
}

#pragma warn (unused-param, off)
void bcd_score_add_5_2 (unsigned char d1, unsigned char d2) {
	/*
	score_offset = score_offsets [pindex];
	bcd_score [score_offset + 0] += d2; if (bcd_score [score_offset + 0] > 14) { bcd_score [score_offset + 1] ++; bcd_score [score_offset + 0] -= 10; }
	bcd_score [score_offset + 1] += d1; if (bcd_score [score_offset + 1] > 14) { bcd_score [score_offset + 2] ++; bcd_score [score_offset + 1] -= 10; }
	if (bcd_score [score_offset + 2] > 14) { bcd_score [score_offset + 3] ++; bcd_score [score_offset + 2] -= 10; signal10000 = pindex; }
	if (bcd_score [score_offset + 3] > 14) { bcd_score [score_offset + 4] ++; bcd_score [score_offset + 3] -= 10; }
	if (bcd_score [score_offset + 4] > 14) { bcd_score [score_offset + 4] -= 10; }
	*/

	__asm__ ("ldx %v", pindex);
	__asm__ ("lda %v, x", score_offsets);
	__asm__ ("tax");

	__asm__ ("ldy #0");
	__asm__ ("clc");
	__asm__ ("lda (sp), y");
	__asm__ ("adc %v+0, x", bcd_score);
	__asm__ ("cmp #26"); 			// "A - 26"
	__asm__ ("bcc %g", BSA52S1); 	// A <= 26
	__asm__ ("inc %v+1, x", bcd_score);
	__asm__ ("sec");
	// __asm__ ("lda %v+0, x", bcd_score); // Already loaded!
	__asm__ ("sbc #10");
BSA52S1:
	__asm__ ("sta %v+0, x", bcd_score);

	__asm__ ("iny");
	__asm__ ("clc");
	__asm__ ("lda (sp), y");
	__asm__ ("adc %v+1, x", bcd_score);
	__asm__ ("cmp #26");
	__asm__ ("bcc %g", BSA52S2);
	__asm__ ("inc %v+2, x", bcd_score);
	__asm__ ("sec");
	__asm__ ("sbc #10");
BSA52S2:
	__asm__ ("sta %v+1, x", bcd_score);

	__asm__ ("lda %v+2, x", bcd_score);
	__asm__ ("cmp #26");
	__asm__ ("bcc %g", BSA52S3);
	__asm__ ("inc %v+3, x", bcd_score);
	__asm__ ("sec");
	__asm__ ("sbc #10");
	__asm__ ("sta %v+2, x", bcd_score);
	__asm__ ("lda #1");
	__asm__ ("sta %v", signal10000);
BSA52S3:

	__asm__ ("lda %v+3, x", bcd_score);
	__asm__ ("cmp #26");
	__asm__ ("bcc %g", BSA52S4);
	__asm__ ("inc %v+4, x", bcd_score);
	__asm__ ("sec");
	__asm__ ("sbc #10");
	__asm__ ("sta %v+3, x", bcd_score);
BSA52S4:

	__asm__ ("lda %v+4, x", bcd_score);
	__asm__ ("cmp #26");
	__asm__ ("bcc %g", BSA52S5);
	__asm__ ("sec");
	__asm__ ("sbc #10");
	__asm__ ("sta %v+4, x", bcd_score);
BSA52S5:
	;
}

void bcd_high_score_update (void) {
	// Compare score and high score:
	for (gpjt = 0; gpjt < 10; gpjt += 5) {
		gpit = 5; while (gpit) {
			--gpit; if (bcd_high_score [gpit] > bcd_score [gpjt + gpit]) continue;
		}

		// Update
		memcpy (bcd_score + gpjt, bcd_high_score, 5);
	}
}

unsigned char bcd_compare_scores (void) {
	// Return which player won
	gpit = 5; while (gpit) { --gpit; 
		rda = bcd_score [5 + gpit];
		rdb = bcd_score [gpit];
		if (rda > rdb) return 1;
		if (rda < rdb) return 0;
	}
	return 2; 	// Nobody
}

void bcd_print (void) {
	gpit = 5; while (gpit) { -- gpit;
		vram_put (gp_gen [gpit]);
	}
	vram_put (DIGIT (0));
}

void bcd_print_ul (void) {
	gpit = 5; while (gpit) { -- gpit;
		*ul = gp_gen [gpit]; ++ ul;
	}
}
