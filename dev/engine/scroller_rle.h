// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// Scroller

void scroll_init (void) {
	cam_pos = MAP_H<<4;
	cam_pos_bit_4 = cam_pos & 0x10;
	cam_pos_bit_3 = cam_pos & 0x08;
	o_scroll_y = scroll_y = SCROLL_Y + SPLIT_SIZE;
	wtp = 0;
	rle_ctr = 0;
	gbuffer_y = 0;
	map_tilepos = (MAP_H&0xff);	
	scroll_state = 8;
	rle_buffer_ptr = SCREEN_BUFFER;

	// For this game, these values are fixed
	__asm__ ("lda #$23");
	__asm__ ("sta %v", ATTRIB_WRITE);
	__asm__ ("lda #$04");
	__asm__ ("sta %v+1", ATTRIB_READ);
		
}

/*
unsigned char scroll_get_byte (void) {
	// Gets a new byte from the rle'd stream.
	
	if (rle_ctr >= 32) {
		rle_ctr -= 32;
	} else {
		rda = *map_ptr ++;
		rle_ctr = rda;
		rle_value = rda & 0x1f;
	}
	return rle_value;
}
*/

unsigned char scroll_get_byte (void) {
		__asm__ ("lda %v", rle_ctr);
		__asm__ ("cmp #32");
		__asm__ ("bcc %g", scroll_get_byte_read);

		__asm__ ("sec");
		__asm__ ("sbc #32");
		__asm__ ("sta %v", rle_ctr);
		__asm__ ("jmp %g", scroll_get_byte_return);	

	scroll_get_byte_read:
		__asm__ ("ldy #0");
		__asm__ ("lda (%v), y", map_ptr);
		__asm__ ("sta %v", rda);
		__asm__ ("sta %v", rle_ctr);

		// Increment map_ptr
		__asm__ ("inc %v", map_ptr);
		__asm__ ("bne %g", scroll_get_byte_inc_done);
		__asm__ ("inc %v+1", map_ptr);
	scroll_get_byte_inc_done:

		__asm__ ("and #$1f");
		__asm__ ("sta %v", rle_value);
	
	scroll_get_byte_return:		
		return rle_value;
}

void scroll_writers_realloc (void) {
	if (wtp) wtp --; else wtp = 29;
		__asm__ ("ldx %v", wtp);

		__asm__ ("lda %v, x", wp_addr_hi);
		__asm__ ("sta %v", SCROLL_WRITE);
		__asm__ ("lda %v, x", wp_addr_lo);
		__asm__ ("sta %v+1", SCROLL_WRITE);

		__asm__ ("lda %v, x", shr2);
		__asm__ ("tay");

		__asm__ ("lda %v, y", attrs_adds); 
		__asm__ ("sta %v+1", ATTRIB_WRITE);
		__asm__ ("lda %v, y", attr_buff_adds);
		__asm__ ("sta %v", ATTRIB_READ);
}

void scroll_advance (unsigned char pixels) {
	o_scroll_y = scroll_y;
	scroll_y -= pixels;
	if (scroll_y > 240) scroll_y += 240;
	cam_pos -= pixels;
	cam_pos_lsb = LSB (cam_pos);
}

void scroll_paint (void) {	
	// Get everything going when needed
	rda = cam_pos & 0x10;
	if (rda != cam_pos_bit_4) {		
		cam_pos_bit_4 = rda;
		cam_pos_bit_3 = cam_pos & 0x08;
			
		scroll_state = 0;
	}

	switch (scroll_state) {
		case 0:
			
			// Prepare to fill buffer from RLE stream
			-- map_tilepos;
			gbuffer_y -= 16;

			rle_buffer_ptr = SCREEN_BUFFER + gbuffer_y;
			attr_buffer_i = (gbuffer_y >> 5);
			gppi = rle_buffer_i = 0;

			scroll_writers_realloc ();

			// No, no break...
		case 1:

			/*
			rle_buffer_ptr [rle_buffer_i] = scroll_get_byte (); ++ rle_buffer_i;
			rle_buffer_ptr [rle_buffer_i] = scroll_get_byte (); ++ rle_buffer_i;
			rle_buffer_ptr [rle_buffer_i] = scroll_get_byte (); ++ rle_buffer_i;
			rle_buffer_ptr [rle_buffer_i] = scroll_get_byte (); ++ rle_buffer_i;
			rle_buffer_ptr [rle_buffer_i] = scroll_get_byte (); ++ rle_buffer_i;
			rle_buffer_ptr [rle_buffer_i] = scroll_get_byte (); ++ rle_buffer_i;
			rle_buffer_ptr [rle_buffer_i] = scroll_get_byte (); ++ rle_buffer_i;
			rle_buffer_ptr [rle_buffer_i] = scroll_get_byte (); ++ rle_buffer_i;
			*/

			__asm__ ("jsr %v", scroll_get_byte);
			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("sta (%v), y", rle_buffer_ptr);
			__asm__ ("inc %v", rle_buffer_i);

			__asm__ ("jsr %v", scroll_get_byte);
			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("sta (%v), y", rle_buffer_ptr);
			__asm__ ("inc %v", rle_buffer_i);

			__asm__ ("jsr %v", scroll_get_byte);
			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("sta (%v), y", rle_buffer_ptr);
			__asm__ ("inc %v", rle_buffer_i);

			__asm__ ("jsr %v", scroll_get_byte);
			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("sta (%v), y", rle_buffer_ptr);
			__asm__ ("inc %v", rle_buffer_i);

			__asm__ ("jsr %v", scroll_get_byte);
			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("sta (%v), y", rle_buffer_ptr);
			__asm__ ("inc %v", rle_buffer_i);

			__asm__ ("jsr %v", scroll_get_byte);
			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("sta (%v), y", rle_buffer_ptr);
			__asm__ ("inc %v", rle_buffer_i);

			__asm__ ("jsr %v", scroll_get_byte);
			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("sta (%v), y", rle_buffer_ptr);
			__asm__ ("inc %v", rle_buffer_i);

			__asm__ ("jsr %v", scroll_get_byte);
			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("sta (%v), y", rle_buffer_ptr);
			__asm__ ("inc %v", rle_buffer_i);

			++ scroll_state;
			if (scroll_state == 2) rle_buffer_i = 0;

			break;

		case 2:
		case 3:
			// Copy half a semiline. This is the bottom half

			// Unrolled, 8 times	
			/*
			rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_2 [rda]; ++ gppi;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_3 [rda]; ++ gppi;
			rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_2 [rda]; ++ gppi;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_3 [rda]; ++ gppi;
			rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_2 [rda]; ++ gppi;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_3 [rda]; ++ gppi;
			rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_2 [rda]; ++ gppi;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_3 [rda]; ++ gppi;
			rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_2 [rda]; ++ gppi;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_3 [rda]; ++ gppi;
			rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_2 [rda]; ++ gppi;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_3 [rda]; ++ gppi;
			rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_2 [rda]; ++ gppi;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_3 [rda]; ++ gppi;
			rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_2 [rda]; ++ gppi;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_3 [rda]; ++ gppi;
			*/

			__asm__ ("ldx %v", gppi);

			// X = gppi

			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("lda (%v), y", rle_buffer_ptr);
			__asm__ ("tay");
			__asm__ ("inc %v", rle_buffer_i);

			// Y = rle_buffer_ptr [rle_buffer_i]

			__asm__ ("lda (%v), y", c_ts_tmaps_2);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			__asm__ ("lda (%v), y", c_ts_tmaps_3);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			// X = gppi

			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("lda (%v), y", rle_buffer_ptr);
			__asm__ ("tay");
			__asm__ ("inc %v", rle_buffer_i);

			// Y = rle_buffer_ptr [rle_buffer_i]

			__asm__ ("lda (%v), y", c_ts_tmaps_2);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			__asm__ ("lda (%v), y", c_ts_tmaps_3);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			// X = gppi

			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("lda (%v), y", rle_buffer_ptr);
			__asm__ ("tay");
			__asm__ ("inc %v", rle_buffer_i);

			// Y = rle_buffer_ptr [rle_buffer_i]

			__asm__ ("lda (%v), y", c_ts_tmaps_2);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			__asm__ ("lda (%v), y", c_ts_tmaps_3);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			// X = gppi

			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("lda (%v), y", rle_buffer_ptr);
			__asm__ ("tay");
			__asm__ ("inc %v", rle_buffer_i);

			// Y = rle_buffer_ptr [rle_buffer_i]

			__asm__ ("lda (%v), y", c_ts_tmaps_2);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			__asm__ ("lda (%v), y", c_ts_tmaps_3);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			// X = gppi

			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("lda (%v), y", rle_buffer_ptr);
			__asm__ ("tay");
			__asm__ ("inc %v", rle_buffer_i);

			// Y = rle_buffer_ptr [rle_buffer_i]

			__asm__ ("lda (%v), y", c_ts_tmaps_2);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			__asm__ ("lda (%v), y", c_ts_tmaps_3);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			// X = gppi

			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("lda (%v), y", rle_buffer_ptr);
			__asm__ ("tay");
			__asm__ ("inc %v", rle_buffer_i);

			// Y = rle_buffer_ptr [rle_buffer_i]

			__asm__ ("lda (%v), y", c_ts_tmaps_2);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			__asm__ ("lda (%v), y", c_ts_tmaps_3);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			// X = gppi

			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("lda (%v), y", rle_buffer_ptr);
			__asm__ ("tay");
			__asm__ ("inc %v", rle_buffer_i);

			// Y = rle_buffer_ptr [rle_buffer_i]

			__asm__ ("lda (%v), y", c_ts_tmaps_2);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			__asm__ ("lda (%v), y", c_ts_tmaps_3);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			// X = gppi

			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("lda (%v), y", rle_buffer_ptr);
			__asm__ ("tay");
			__asm__ ("inc %v", rle_buffer_i);

			// Y = rle_buffer_ptr [rle_buffer_i]

			__asm__ ("lda (%v), y", c_ts_tmaps_2);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			__asm__ ("lda (%v), y", c_ts_tmaps_3);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			__asm__ ("stx %v", gppi);

			++ scroll_state;
			break;

		case 4:
			// Paint attributes
			rle_buffer_i = 0;

			// Update top or bottom half of attributes
			// depending on bit 4 of cam_pos
			if (wtp & 2) {

				// Unrolled
				/*
				rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				rdb = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				ATTRIB_BUFFER [gpri] = (ATTRIB_BUFFER [gpri] & 0x0f) | c_ts_pals_2 [rda] | c_ts_pals_3 [rdb];
				++ gpri;
				rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				rdb = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				ATTRIB_BUFFER [gpri] = (ATTRIB_BUFFER [gpri] & 0x0f) | c_ts_pals_2 [rda] | c_ts_pals_3 [rdb];
				++ gpri;
				rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				rdb = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				ATTRIB_BUFFER [gpri] = (ATTRIB_BUFFER [gpri] & 0x0f) | c_ts_pals_2 [rda] | c_ts_pals_3 [rdb];
				++ gpri;
				rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				rdb = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				ATTRIB_BUFFER [gpri] = (ATTRIB_BUFFER [gpri] & 0x0f) | c_ts_pals_2 [rda] | c_ts_pals_3 [rdb];
				++ gpri;
				rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				rdb = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				ATTRIB_BUFFER [gpri] = (ATTRIB_BUFFER [gpri] & 0x0f) | c_ts_pals_2 [rda] | c_ts_pals_3 [rdb];
				++ gpri;
				rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				rdb = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				ATTRIB_BUFFER [gpri] = (ATTRIB_BUFFER [gpri] & 0x0f) | c_ts_pals_2 [rda] | c_ts_pals_3 [rdb];
				++ gpri;
				rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				rdb = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				ATTRIB_BUFFER [gpri] = (ATTRIB_BUFFER [gpri] & 0x0f) | c_ts_pals_2 [rda] | c_ts_pals_3 [rdb];
				++ gpri;
				rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				rdb = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				ATTRIB_BUFFER [gpri] = (ATTRIB_BUFFER [gpri] & 0x0f) | c_ts_pals_2 [rda] | c_ts_pals_3 [rdb];
				++ gpri;
				*/

				__asm__ ("ldx %v", ATTRIB_READ);

				// Roll dice

				__asm__ ("ldy %v", rle_buffer_i);
				
				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rda);
				__asm__ ("iny");

				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rdb);
				__asm__ ("iny");

				__asm__ ("sty %v", rle_buffer_i);

				__asm__ ("lda %w, x", ATTRIB_BUFFER_V);
				__asm__ ("and #$0f");

				__asm__ ("ldy %v", rda);
				__asm__ ("ora (%v), y", c_ts_pals_2);

				__asm__ ("ldy %v", rdb);
				__asm__ ("ora (%v), y", c_ts_pals_3);

				__asm__ ("sta %w, x", ATTRIB_BUFFER_V);

				__asm__ ("inx");

				// Roll dice

				__asm__ ("ldy %v", rle_buffer_i);
				
				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rda);
				__asm__ ("iny");

				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rdb);
				__asm__ ("iny");

				__asm__ ("sty %v", rle_buffer_i);

				__asm__ ("lda %w, x", ATTRIB_BUFFER_V);
				__asm__ ("and #$0f");

				__asm__ ("ldy %v", rda);
				__asm__ ("ora (%v), y", c_ts_pals_2);

				__asm__ ("ldy %v", rdb);
				__asm__ ("ora (%v), y", c_ts_pals_3);

				__asm__ ("sta %w, x", ATTRIB_BUFFER_V);

				__asm__ ("inx");

				// Roll dice

				__asm__ ("ldy %v", rle_buffer_i);
				
				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rda);
				__asm__ ("iny");

				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rdb);
				__asm__ ("iny");

				__asm__ ("sty %v", rle_buffer_i);

				__asm__ ("lda %w, x", ATTRIB_BUFFER_V);
				__asm__ ("and #$0f");

				__asm__ ("ldy %v", rda);
				__asm__ ("ora (%v), y", c_ts_pals_2);

				__asm__ ("ldy %v", rdb);
				__asm__ ("ora (%v), y", c_ts_pals_3);

				__asm__ ("sta %w, x", ATTRIB_BUFFER_V);

				__asm__ ("inx");

				// Roll dice

				__asm__ ("ldy %v", rle_buffer_i);
				
				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rda);
				__asm__ ("iny");

				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rdb);
				__asm__ ("iny");

				__asm__ ("sty %v", rle_buffer_i);

				__asm__ ("lda %w, x", ATTRIB_BUFFER_V);
				__asm__ ("and #$0f");

				__asm__ ("ldy %v", rda);
				__asm__ ("ora (%v), y", c_ts_pals_2);

				__asm__ ("ldy %v", rdb);
				__asm__ ("ora (%v), y", c_ts_pals_3);

				__asm__ ("sta %w, x", ATTRIB_BUFFER_V);

				__asm__ ("inx");

				// Roll dice

				__asm__ ("ldy %v", rle_buffer_i);
				
				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rda);
				__asm__ ("iny");

				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rdb);
				__asm__ ("iny");

				__asm__ ("sty %v", rle_buffer_i);

				__asm__ ("lda %w, x", ATTRIB_BUFFER_V);
				__asm__ ("and #$0f");

				__asm__ ("ldy %v", rda);
				__asm__ ("ora (%v), y", c_ts_pals_2);

				__asm__ ("ldy %v", rdb);
				__asm__ ("ora (%v), y", c_ts_pals_3);

				__asm__ ("sta %w, x", ATTRIB_BUFFER_V);

				__asm__ ("inx");

				// Roll dice

				__asm__ ("ldy %v", rle_buffer_i);
				
				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rda);
				__asm__ ("iny");

				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rdb);
				__asm__ ("iny");

				__asm__ ("sty %v", rle_buffer_i);

				__asm__ ("lda %w, x", ATTRIB_BUFFER_V);
				__asm__ ("and #$0f");

				__asm__ ("ldy %v", rda);
				__asm__ ("ora (%v), y", c_ts_pals_2);

				__asm__ ("ldy %v", rdb);
				__asm__ ("ora (%v), y", c_ts_pals_3);

				__asm__ ("sta %w, x", ATTRIB_BUFFER_V);

				__asm__ ("inx");

				// Roll dice

				__asm__ ("ldy %v", rle_buffer_i);
				
				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rda);
				__asm__ ("iny");

				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rdb);
				__asm__ ("iny");

				__asm__ ("sty %v", rle_buffer_i);

				__asm__ ("lda %w, x", ATTRIB_BUFFER_V);
				__asm__ ("and #$0f");

				__asm__ ("ldy %v", rda);
				__asm__ ("ora (%v), y", c_ts_pals_2);

				__asm__ ("ldy %v", rdb);
				__asm__ ("ora (%v), y", c_ts_pals_3);

				__asm__ ("sta %w, x", ATTRIB_BUFFER_V);

				__asm__ ("inx");

				// Roll dice

				__asm__ ("ldy %v", rle_buffer_i);
				
				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rda);
				__asm__ ("iny");

				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rdb);
				__asm__ ("iny");

				__asm__ ("sty %v", rle_buffer_i);

				__asm__ ("lda %w, x", ATTRIB_BUFFER_V);
				__asm__ ("and #$0f");

				__asm__ ("ldy %v", rda);
				__asm__ ("ora (%v), y", c_ts_pals_2);

				__asm__ ("ldy %v", rdb);
				__asm__ ("ora (%v), y", c_ts_pals_3);

				__asm__ ("sta %w, x", ATTRIB_BUFFER_V);

				__asm__ ("inx");


			} else {				

				// Unrolled				
				/*
				rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				rdb = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				ATTRIB_BUFFER [gpri] = (ATTRIB_BUFFER [gpri] & 0xf0) | c_ts_pals_0 [rda] | c_ts_pals_1 [rdb];
				++ gpri;
				rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				rdb = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				ATTRIB_BUFFER [gpri] = (ATTRIB_BUFFER [gpri] & 0xf0) | c_ts_pals_0 [rda] | c_ts_pals_1 [rdb];
				++ gpri;
				rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				rdb = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				ATTRIB_BUFFER [gpri] = (ATTRIB_BUFFER [gpri] & 0xf0) | c_ts_pals_0 [rda] | c_ts_pals_1 [rdb];
				++ gpri;
				rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				rdb = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				ATTRIB_BUFFER [gpri] = (ATTRIB_BUFFER [gpri] & 0xf0) | c_ts_pals_0 [rda] | c_ts_pals_1 [rdb];
				++ gpri;
				rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				rdb = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				ATTRIB_BUFFER [gpri] = (ATTRIB_BUFFER [gpri] & 0xf0) | c_ts_pals_0 [rda] | c_ts_pals_1 [rdb];
				++ gpri;
				rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				rdb = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				ATTRIB_BUFFER [gpri] = (ATTRIB_BUFFER [gpri] & 0xf0) | c_ts_pals_0 [rda] | c_ts_pals_1 [rdb];
				++ gpri;
				rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				rdb = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				ATTRIB_BUFFER [gpri] = (ATTRIB_BUFFER [gpri] & 0xf0) | c_ts_pals_0 [rda] | c_ts_pals_1 [rdb];
				++ gpri;
				rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				rdb = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
				ATTRIB_BUFFER [gpri] = (ATTRIB_BUFFER [gpri] & 0xf0) | c_ts_pals_0 [rda] | c_ts_pals_1 [rdb];
				++ gpri;								
				*/

				__asm__ ("ldx %v", ATTRIB_READ);

				// Roll dice

				__asm__ ("ldy %v", rle_buffer_i);
				
				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rda);
				__asm__ ("iny");

				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rdb);
				__asm__ ("iny");

				__asm__ ("sty %v", rle_buffer_i);

				__asm__ ("lda %w, x", ATTRIB_BUFFER_V);
				__asm__ ("and #$f0");

				__asm__ ("ldy %v", rda);
				__asm__ ("ora (%v), y", c_ts_pals_0);

				__asm__ ("ldy %v", rdb);
				__asm__ ("ora (%v), y", c_ts_pals_1);

				__asm__ ("sta %w, x", ATTRIB_BUFFER_V);

				__asm__ ("inx");

				// Roll dice

				__asm__ ("ldy %v", rle_buffer_i);
				
				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rda);
				__asm__ ("iny");

				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rdb);
				__asm__ ("iny");

				__asm__ ("sty %v", rle_buffer_i);

				__asm__ ("lda %w, x", ATTRIB_BUFFER_V);
				__asm__ ("and #$f0");

				__asm__ ("ldy %v", rda);
				__asm__ ("ora (%v), y", c_ts_pals_0);

				__asm__ ("ldy %v", rdb);
				__asm__ ("ora (%v), y", c_ts_pals_1);

				__asm__ ("sta %w, x", ATTRIB_BUFFER_V);

				__asm__ ("inx");

				// Roll dice

				__asm__ ("ldy %v", rle_buffer_i);
				
				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rda);
				__asm__ ("iny");

				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rdb);
				__asm__ ("iny");

				__asm__ ("sty %v", rle_buffer_i);

				__asm__ ("lda %w, x", ATTRIB_BUFFER_V);
				__asm__ ("and #$f0");

				__asm__ ("ldy %v", rda);
				__asm__ ("ora (%v), y", c_ts_pals_0);

				__asm__ ("ldy %v", rdb);
				__asm__ ("ora (%v), y", c_ts_pals_1);

				__asm__ ("sta %w, x", ATTRIB_BUFFER_V);

				__asm__ ("inx");

				// Roll dice

				__asm__ ("ldy %v", rle_buffer_i);
				
				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rda);
				__asm__ ("iny");

				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rdb);
				__asm__ ("iny");

				__asm__ ("sty %v", rle_buffer_i);

				__asm__ ("lda %w, x", ATTRIB_BUFFER_V);
				__asm__ ("and #$f0");

				__asm__ ("ldy %v", rda);
				__asm__ ("ora (%v), y", c_ts_pals_0);

				__asm__ ("ldy %v", rdb);
				__asm__ ("ora (%v), y", c_ts_pals_1);

				__asm__ ("sta %w, x", ATTRIB_BUFFER_V);

				__asm__ ("inx");

				// Roll dice

				__asm__ ("ldy %v", rle_buffer_i);
				
				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rda);
				__asm__ ("iny");

				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rdb);
				__asm__ ("iny");

				__asm__ ("sty %v", rle_buffer_i);

				__asm__ ("lda %w, x", ATTRIB_BUFFER_V);
				__asm__ ("and #$f0");

				__asm__ ("ldy %v", rda);
				__asm__ ("ora (%v), y", c_ts_pals_0);

				__asm__ ("ldy %v", rdb);
				__asm__ ("ora (%v), y", c_ts_pals_1);

				__asm__ ("sta %w, x", ATTRIB_BUFFER_V);

				__asm__ ("inx");

				// Roll dice

				__asm__ ("ldy %v", rle_buffer_i);
				
				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rda);
				__asm__ ("iny");

				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rdb);
				__asm__ ("iny");

				__asm__ ("sty %v", rle_buffer_i);

				__asm__ ("lda %w, x", ATTRIB_BUFFER_V);
				__asm__ ("and #$f0");

				__asm__ ("ldy %v", rda);
				__asm__ ("ora (%v), y", c_ts_pals_0);

				__asm__ ("ldy %v", rdb);
				__asm__ ("ora (%v), y", c_ts_pals_1);

				__asm__ ("sta %w, x", ATTRIB_BUFFER_V);

				__asm__ ("inx");

				// Roll dice

				__asm__ ("ldy %v", rle_buffer_i);
				
				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rda);
				__asm__ ("iny");

				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rdb);
				__asm__ ("iny");

				__asm__ ("sty %v", rle_buffer_i);

				__asm__ ("lda %w, x", ATTRIB_BUFFER_V);
				__asm__ ("and #$f0");

				__asm__ ("ldy %v", rda);
				__asm__ ("ora (%v), y", c_ts_pals_0);

				__asm__ ("ldy %v", rdb);
				__asm__ ("ora (%v), y", c_ts_pals_1);

				__asm__ ("sta %w, x", ATTRIB_BUFFER_V);

				__asm__ ("inx");

				// Roll dice

				__asm__ ("ldy %v", rle_buffer_i);
				
				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rda);
				__asm__ ("iny");

				__asm__ ("lda (%v), y", rle_buffer_ptr);
				__asm__ ("sta %v", rdb);
				__asm__ ("iny");

				__asm__ ("sty %v", rle_buffer_i);

				__asm__ ("lda %w, x", ATTRIB_BUFFER_V);
				__asm__ ("and #$f0");

				__asm__ ("ldy %v", rda);
				__asm__ ("ora (%v), y", c_ts_pals_0);

				__asm__ ("ldy %v", rdb);
				__asm__ ("ora (%v), y", c_ts_pals_1);

				__asm__ ("sta %w, x", ATTRIB_BUFFER_V);

				__asm__ ("inx");
			}

			++ scroll_state;
			break;

		case 5:
			// wait until we need a new semiline

			rda = cam_pos & 0x08;
			if (rda != cam_pos_bit_3) {
				cam_pos_bit_3 = rda;
				
				gpri = gppi = rle_buffer_i = 0;

				// Relocate writers
				scroll_writers_realloc ();

				++ scroll_state;
			}

			break;

		case 6:
		case 7:
			// Copy half a semiline. This is the top half
			// Unrolled, 8 times

			/*
			rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_0 [rda]; ++ gppi;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_1 [rda]; ++ gppi;
			rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_0 [rda]; ++ gppi;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_1 [rda]; ++ gppi;
			rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_0 [rda]; ++ gppi;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_1 [rda]; ++ gppi;
			rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_0 [rda]; ++ gppi;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_1 [rda]; ++ gppi;
			rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_0 [rda]; ++ gppi;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_1 [rda]; ++ gppi;
			rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_0 [rda]; ++ gppi;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_1 [rda]; ++ gppi;
			rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_0 [rda]; ++ gppi;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_1 [rda]; ++ gppi;
			rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_0 [rda]; ++ gppi;
			SCROLL_BUFFER [gppi] = c_ts_tmaps_1 [rda]; ++ gppi;
			*/

			__asm__ ("ldx %v", gppi);

			// X = gppi

			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("lda (%v), y", rle_buffer_ptr);
			__asm__ ("tay");
			__asm__ ("inc %v", rle_buffer_i);

			// Y = rle_buffer_ptr [rle_buffer_i]

			__asm__ ("lda (%v), y", c_ts_tmaps_0);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			__asm__ ("lda (%v), y", c_ts_tmaps_1);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			// X = gppi

			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("lda (%v), y", rle_buffer_ptr);
			__asm__ ("tay");
			__asm__ ("inc %v", rle_buffer_i);

			// Y = rle_buffer_ptr [rle_buffer_i]

			__asm__ ("lda (%v), y", c_ts_tmaps_0);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			__asm__ ("lda (%v), y", c_ts_tmaps_1);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			// X = gppi

			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("lda (%v), y", rle_buffer_ptr);
			__asm__ ("tay");
			__asm__ ("inc %v", rle_buffer_i);

			// Y = rle_buffer_ptr [rle_buffer_i]

			__asm__ ("lda (%v), y", c_ts_tmaps_0);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			__asm__ ("lda (%v), y", c_ts_tmaps_1);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			// X = gppi

			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("lda (%v), y", rle_buffer_ptr);
			__asm__ ("tay");
			__asm__ ("inc %v", rle_buffer_i);

			// Y = rle_buffer_ptr [rle_buffer_i]

			__asm__ ("lda (%v), y", c_ts_tmaps_0);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			__asm__ ("lda (%v), y", c_ts_tmaps_1);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			// X = gppi

			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("lda (%v), y", rle_buffer_ptr);
			__asm__ ("tay");
			__asm__ ("inc %v", rle_buffer_i);

			// Y = rle_buffer_ptr [rle_buffer_i]

			__asm__ ("lda (%v), y", c_ts_tmaps_0);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			__asm__ ("lda (%v), y", c_ts_tmaps_1);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			// X = gppi

			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("lda (%v), y", rle_buffer_ptr);
			__asm__ ("tay");
			__asm__ ("inc %v", rle_buffer_i);

			// Y = rle_buffer_ptr [rle_buffer_i]

			__asm__ ("lda (%v), y", c_ts_tmaps_0);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			__asm__ ("lda (%v), y", c_ts_tmaps_1);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			// X = gppi

			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("lda (%v), y", rle_buffer_ptr);
			__asm__ ("tay");
			__asm__ ("inc %v", rle_buffer_i);

			// Y = rle_buffer_ptr [rle_buffer_i]

			__asm__ ("lda (%v), y", c_ts_tmaps_0);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			__asm__ ("lda (%v), y", c_ts_tmaps_1);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			// X = gppi

			__asm__ ("ldy %v", rle_buffer_i);
			__asm__ ("lda (%v), y", rle_buffer_ptr);
			__asm__ ("tay");
			__asm__ ("inc %v", rle_buffer_i);

			// Y = rle_buffer_ptr [rle_buffer_i]

			__asm__ ("lda (%v), y", c_ts_tmaps_0);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			__asm__ ("lda (%v), y", c_ts_tmaps_1);
			__asm__ ("sta %w, x", SCROLL_BUFFER_V);
			__asm__ ("inx");

			__asm__ ("stx %v", gppi);		

			++ scroll_state;
			break;
	}
}
