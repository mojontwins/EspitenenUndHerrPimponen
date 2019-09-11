// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

void enems_do (void) {

	aptgmx [0] = apgotten [0] = 
	aptgmx [1] = apgotten [1] = 0;

	enit = enstart;
	enjt = ENEMS_MAX; 

	// while (enjt --) {

	enems_do_loop:
		__asm__ ("lda %v", enjt);
		__asm__ ("beq %g", enems_do_loop_done);
		__asm__ ("dec %v", enjt);

		//enit += (ENEMS_MAX-1); if (enit >= ENEMS_MAX) enit -= ENEMS_MAX;
		__asm__ ("lda %v", enit);
		__asm__ ("clc");
		__asm__ ("adc #%b", (ENEMS_MAX - 1));
		
		__asm__ ("cmp #%b", ENEMS_MAX);
		__asm__ ("bcc %g", EDCL0);

		__asm__ ("sec");
		__asm__ ("sbc #%b", ENEMS_MAX);

	EDCL0:
		__asm__ ("sta %v", enit);

		__asm__ ("tay");

		// Y -> enit

		// if (ena [enit])
		__asm__ ("lda %v, y", ena);
		__asm__ ("beq %g", enems_do_loop);

		// Local temporary copy in ZP 
		__asm__ ("lda %v, y", enct);
		__asm__ ("sta %v", _enct);

		__asm__ ("lda %v, y", enx);
		__asm__ ("sta %v", _enx);

		__asm__ ("lda %v, y", enx1);
		__asm__ ("sta %v", _enx1);

		__asm__ ("lda %v, y", enx2);
		__asm__ ("sta %v", _enx2);

		__asm__ ("lda %v, y", enmx);
		__asm__ ("sta %v", _enmx);

		__asm__ ("lda %v, y", enmy);
		__asm__ ("sta %v", _enmy);

		// 16 bits

		__asm__ ("tya");
		__asm__ ("asl a");
		__asm__ ("tax");

		__asm__ ("lda %v, x", eny);
		__asm__ ("sta %v", _eny);
		__asm__ ("lda %v+1, x", eny);
		__asm__ ("sta %v+1", _eny);

		// Blah

		__asm__ ("lda #0");
		__asm__ ("sta %v", envx);
		__asm__ ("sta %v", envy);

		__asm__ ("lda %v, y", ent);
		__asm__ ("sta %v", rdt);

	// ==========================================================

		// Pre
		__asm__ ("cmp #3");
		__asm__ ("beq %g", enems_do_pre_type_3);
		__asm__ ("cmp #2");
		__asm__ ("beq %g", enems_do_pre_type_2);
		__asm__ ("cmp #7");
		__asm__ ("beq %g", enems_do_pre_type_7);
		__asm__ ("cmp #4");
		__asm__ ("beq %g", enems_do_pre_type_4);
		__asm__ ("cmp #6");
		__asm__ ("beq %g", enems_do_pre_type_6);

		__asm__ ("jmp %g", enems_do_pre_done);

	// ----------------------------------------------------------

	enems_do_pre_type_3:
		// Gravity
		__asm__ ("ldx %v", _enmy);
		__asm__ ("lda %v, x", enems_lut_falling);
		__asm__ ("sta %v", envy);
		__asm__ ("txa");
		__asm__ ("cmp #%b", ENEMS_LUT_FALLING_MAXIDX);
		__asm__ ("beq %g", EDPT3S0);
		__asm__ ("inc %v", _enmy);
	EDPT3S0:

	// ----------------------------------------------------------

	enems_do_pre_type_2:
		// Move left / right
		//if (_enx1 || half_life ) 
		__asm__ ("lda %v", _enx1);
		__asm__ ("bne %g", EDPT2D);

		__asm__ ("lda %v", half_life);
		__asm__ ("bne %g", enems_do_pre_done);

	EDPT2D:
		__asm__ ("lda %v", _enmx);
		__asm__ ("sta %v", envx);

		__asm__ ("jmp %g", enems_do_pre_done);

	// ----------------------------------------------------------

	enems_do_pre_type_7:
		__asm__ ("lda %v", _enmx);
		__asm__ ("sta %v", envx);

		__asm__ ("jmp %g", enems_do_pre_done);

	// ----------------------------------------------------------

	enems_do_pre_type_4:
		// Pursuers / retreaters
		__asm__ ("lda #1");
		__asm__ ("sta %v", rdd);

		__asm__ ("lda %v", enit);
		__asm__ ("tay");
		__asm__ ("asl a");
		__asm__ ("tax");

		// Retreaters
		// if (eny1 [enit] == 0) {
		// 16 bits
		__asm__ ("lda %v, x", eny1);
		__asm__ ("ora %v+1,x", eny1);
		__asm__ ("bne %g", EDPT4E0);

		// _eny += 2; break;

		__asm__ ("lda %v", eny);
		__asm__ ("clc");
		__asm__ ("adc #2");
		__asm__ ("sta %v", eny);
		__asm__ ("bcc %g", EDPT4C0);
		__asm__ ("inc %v+1", eny);
	EDPT4C0:

		__asm__ ("jmp %g", enems_do_pre_done);

	EDPT4E0:
		// else if ( eny1 [enit] > _eny + 240 ) {
		// _eny + 240 < eny1 [enit]
		// 16 bits
		__asm__ ("lda %v", _eny);
		__asm__ ("clc");
		__asm__ ("adc #240");
		__asm__ ("sta %v", rdu16);
		__asm__ ("lda %v+1", _eny);
		__asm__ ("adc #0");
		__asm__ ("sta %v+1", rdu16);

		__asm__ ("lda %v", rdu16);
		__asm__ ("cmp %v, x", eny1);
		__asm__ ("lda %v+1", rdu16);
		__asm__ ("sbc %v+1, x", eny1);
		__asm__ ("bcs %g", EDPT4E1);

		__asm__ ("lda #0");
		__asm__ ("sta %v, x", eny1);
		__asm__ ("sta %v+1, x", eny1);
	EDPT4E1:

		// Pursue
		// Pursue WHOM?
		// If i'm running this code, either apon [0] or apon [1]
		// or BOTH are 1, so let's take some shortcuts.

		// if (apon [0] && apon [1]) {
		__asm__ ("lda %v", apon);
		__asm__ ("beq %g", EDPT4PE1);

		__asm__ ("lda %v+1", apon);
		__asm__ ("beq %g", EDPT4PE2);

		__asm__ ("tya");
		__asm__ ("and #1");
		
		__asm__ ("jmp %g", EDPT4PEE);

	EDPT4PE1:
		// Only apon [1] is on, so pursue player 2
		__asm__ ("lda #1");
		__asm__ ("jmp %g", EDPT4PEE);

	EDPT4PE2:
		// Only apon [0] is on, so pursue player 1
		__asm__ ("lda #0");

	EDPT4PEE:
		__asm__ ("sta %v", pindex);

		// Let's extract the variables we need:
		__asm__ ("tax");
		__asm__ ("lda %v, x", aprx);
		__asm__ ("sta %v", prx);

		__asm__ ("lda %v, x", apfacing);;
		__asm__ ("sta %v", pfacing);

		// 16 bits
		__asm__ ("txa");
		__asm__ ("asl a");
		__asm__ ("tax");
		__asm__ ("lda %v, x", apry);
		__asm__ ("sta %v", pry);
		__asm__ ("lda %v+1, x", apry);
		__asm__ ("sta %v+1", pry);

		// If the pursued player is facing the enemy,
		// don't move

		//if ((pfacing && prx > _enx) || (!pfacing && prx < _enx)) 
		__asm__ ("lda %v", prx);
		__asm__ ("cmp %v", _enx);
		__asm__ ("bcc %g", EDPT4F0);

		// prx is >= _enx, so if pfacing => break
		__asm__ ("lda %v", pfacing);
		__asm__ ("bne %g", enems_do_pre_done);
		__asm__ ("jmp %g", EDPT4FE);

	EDPT4F0:		
		// prx is < _enx, so if !pfacing => break
		__asm__ ("lda %v", pfacing);
		__asm__ ("beq %g", enems_do_pre_done);

	EDPT4FE:

		__asm__ ("lda #0");
		__asm__ ("sta %v", rdd);

		// don't move all at the same time
		// if ((enit + half_life) & 1) {
		__asm__ ("tya");
		__asm__ ("clc");
		__asm__ ("adc %v", half_life);
		__asm__ ("and #1");
		__asm__ ("bne %g", enems_do_pre_done);

		// Let's ask rand...
		__asm__ ("jsr %v", rand8);
		__asm__ ("sta %v", rda);


		// if (rda > 2) => rda >= 3!
		__asm__ ("cmp #3");
		__asm__ ("bcc %g", EDPT4MXD);

			// if (prx > _enx) -> (_enx < prx), skip if >= (BCS)
			__asm__ ("lda %v", _enx);
			__asm__ ("cmp %v", prx);
			__asm__ ("bcs %g", EDPT4MXADDS);

				// if (_enmx < ENEMS_LUT_ACCELERATION_MAXIDX) _enmx ++;
				// Signed comparison!
				__asm__ ("lda %v", _enmx);
				__asm__ ("sec");
				__asm__ ("sbc #%b", ENEMS_LUT_ACCELERATION_MAXIDX);
				__asm__ ("bvc %g", EDPT4MXADDC0);
				__asm__ ("eor #$80");
			EDPT4MXADDC0:
				__asm__ ("bpl %g", EDPT4MXADDS);

				__asm__ ("inc %v", _enmx);

		EDPT4MXADDS:

			// if (prx < _enx), skip if >= (BCS)
			__asm__ ("lda %v", prx);
			__asm__ ("cmp %v", _enx);
			__asm__ ("bcs %g", EDPT4MXD);

				// if (_enmx > -ENEMS_LUT_ACCELERATION_MAXIDX) -- _enmx;
				// if (-ENEMS_LUT_ACCELERATION_MAXIDX < -enmx)
				// Signed comparison!
				__asm__ ("lda #%b", -ENEMS_LUT_ACCELERATION_MAXIDX);
				__asm__ ("sec");
				__asm__ ("sbc %v", _enmx);
				__asm__ ("bvc %g", EDPT4MXSUBC0);
				__asm__ ("eor #$80");
			EDPT4MXSUBC0:
				__asm__ ("bpl %g", EDPT4MXD);

				__asm__ ("dec %v", _enmx);
	EDPT4MXD:

		envx = ADD_SIGN (
			_enmx,
			enems_lut_acceleration [ABS (_enmx)]
		);
		_enx += envx;

		if (rda > 2) {
			if (pry > _eny) {
				if (_enmy < ENEMS_LUT_ACCELERATION_MAXIDX) _enmy ++;
			}
			if (pry < _eny) {
				if (_enmy > -ENEMS_LUT_ACCELERATION_MAXIDX) _enmy --;
			}
		}	

		envy = ADD_SIGN (
			_enmy,
			enems_lut_acceleration [ABS (_enmy)]
		);
		_eny += envy;

		__asm__ ("jmp %g", enems_do_pre_done);

	// ----------------------------------------------------------

	enems_do_pre_type_6:
		// if (_enct) _enct --; else {
		__asm__ ("lda %v", _enct);
		__asm__ ("beq %g", EDPT6S);

		__asm__ ("dec %v", _enct);
		__asm__ ("jmp %g", EDPT6E);

	EDPT6S:
		// _ENSTATE = !_ENSTATE;
		__asm__ ("lda %v", _ENSTATE);
		__asm__ ("bne %g", EDPT6SNE);
		__asm__ ("lda #1");		
		__asm__ ("jmp %g", EDPT6SNS);
	EDPT6SNE:
		__asm__ ("lda #0");
	EDPT6SNS:
		__asm__ ("sta %v", _ENSTATE);

		// _enct = _ENSTATE ? 128 : 64;
		__asm__ ("beq %g", EDPT6S64);
		__asm__ ("lda #128");
		__asm__ ("jmp %g", EDPT6SE);
	EDPT6S64:
		__asm__ ("lda #64");
	EDPT6SE:
		__asm__ ("sta %v", _enct);
	EDPT6E:

		//__asm__ ("jmp %g", enems_do_pre_done);
	enems_do_pre_done:

	// ==========================================================

	// Vertical movement & collision

		__asm__ ("lda %v", envy);
		__asm__ ("beq %g", enems_do_vertical_done);

	// _eny += envy;
	// envy is signed 8 bits, needs sign extension
		__asm__ ("ldx #0");				// X = 00		
		__asm__ ("cmp #$80");			// is negative?
		__asm__ ("bcc %g", EVMCP0);		// branch if not
		__asm__ ("dex");				// X = FF
		__asm__ ("clc");
	EVMCP0:				
		__asm__ ("adc %v", _eny);
		__asm__ ("sta %v", _eny);
		__asm__ ("txa");				// Sign extended
		__asm__ ("adc %v+1", _eny);
		__asm__ ("sta %v+1", _eny);

		//__asm__ ("lda #0");
		//__asm__ ("sta %v", enhitbg);

		__asm__ ("lda %v", _enx);
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("sta %v", cx1);

		__asm__ ("lda %v", _enx);
		__asm__ ("clc");
		__asm__ ("adc #7");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("sta %v", cx2);

		// To check if a number is NEGATIVE,
		// we just have to check if bit 7 is 1:
		__asm__ ("lda %v", envy);
		__asm__ ("asl a");				// bit 7 goes to C
		__asm__ ("bcc %g", EVMCNN0);	// Not negative.

		// envy < 0
		// cy2 = cy1 = (_eny) >> 4;
		__asm__ ("lda %v", _eny);
		__asm__ ("ldx %v+1", _eny);
		__asm__ ("jsr shrax4");
		__asm__ ("sta %v", cy1);
		__asm__ ("sta %v", cy2);

		cm_two_points ();
		__asm__ ("ldy %v", enit);

		// if ((at1 & ENEMS_OBSTACLE_BITS) || (at2 & ENEMS_OBSTACLE_BITS)) {
		__asm__ ("lda %v", at1);
		__asm__ ("and #%b", ENEMS_OBSTACLE_BITS);
		__asm__ ("bne %g", EVMCNDO);

		__asm__ ("lda %v", at2);
		__asm__ ("and #%b", ENEMS_OBSTACLE_BITS);
		__asm__ ("beq %g", EVMCNNE);

	EVMCNDO:
		__asm__ ("lda #0");
		__asm__ ("sta %v", _enmy);
		
		__asm__ ("lda %v", cy1);
		__asm__ ("clc");
		__asm__ ("adc #1");
	
		__asm__ ("ldx #0");
		__asm__ ("jsr shlax4");

		__asm__ ("sta %v", _eny);
		__asm__ ("stx %v+1", _eny);

		//__asm__ ("lda #1");
		//__asm__ ("sta %v", enhitbg);

		__asm__ ("jmp %g", EVMCNNE);

	EVMCNN0:
		// envy >= 0
		//__asm__ ("beq %g", EVMCNNE);	// is 0

		// envy > 0
		// cy2 = cy1 = (_eny + 15) >> 4;
		__asm__ ("lda %v", _eny);
		__asm__ ("ldx %v+1", _eny);
		__asm__ ("clc");
		__asm__ ("adc #15");
		__asm__ ("bcc %g", EVMCNN0CC);
		__asm__ ("inx");
	EVMCNN0CC:
		__asm__ ("jsr shrax4");
		__asm__ ("sta %v", cy1);
		__asm__ ("sta %v", cy2);

		cm_two_points ();
		__asm__ ("ldy %v", enit);

		// if ((at1 & ENEMS_FLOOR_BITS) || (at2 & ENEMS_FLOOR_BITS)) {
		__asm__ ("lda %v", at1);
		__asm__ ("and #%b", ENEMS_FLOOR_BITS);
		__asm__ ("bne %g", EVMCPDO);

		__asm__ ("lda %v", at2);
		__asm__ ("and #%b", ENEMS_FLOOR_BITS);
		__asm__ ("beq %g", EVMCNNE);

	EVMCPDO:		

		__asm__ ("lda #0");
		__asm__ ("sta %v", _enmy);
		
		__asm__ ("lda %v", cy2);
		__asm__ ("sec");
		__asm__ ("sbc #1");
		
		__asm__ ("ldx #0");
		__asm__ ("jsr shlax4");

		__asm__ ("sta %v", _eny);
		__asm__ ("stx %v+1", _eny);

		//__asm__ ("lda #1");
		//__asm__ ("sta %v", enhitbg);

	EVMCNNE:
	enems_do_vertical_done:

	// ==========================================================

	// Horizontal movement & collision

		__asm__ ("lda %v", _enx);
		__asm__ ("sta %v", encx);
		__asm__ ("clc");
		__asm__ ("adc %v", envx);
		__asm__ ("sta %v", _enx);

		// Enemy type 7 is special
		__asm__ ("lda %v", rdt);
		__asm__ ("cmp #7");
		__asm__ ("bne %g", EHMCNT7);

		// Type 7
			// if (_enx < _enx1 || _enx > _enx2) {
			// if (_enx < _enx1)
			__asm__ ("lda %v", _enx);
			__asm__ ("cmp %v", _enx1);
			__asm__ ("bcc %g", EHMCT7CH);

			// if (_enx2 < _enx)
			__asm__ ("lda %v", _enx2);
			__asm__ ("cmp %v", _enx);
			__asm__ ("bcs %g", EHMCNT7E);

		EHMCT7CH:
			// _enmx = -_enmx;
			__asm__ ("lda #0");
			__asm__ ("sec");
			__asm__ ("sbc %v", _enmx);
			__asm__ ("sta %v", _enmx);

			__asm__ ("lda %v", encx);
			__asm__ ("sta %v", _enx);

			__asm__ ("jmp %g", EHMCNT7E);

	EHMCNT7:
			// Not type 7. Collide with bg

			// But only if moving
			__asm__ ("lda %v", envx);
			__asm__ ("beq %g", EHMCNT7E);

			// cy1 = _eny >> 4;
			__asm__ ("lda %v", _eny);
			__asm__ ("ldx %v+1", _eny);
			__asm__ ("jsr shrax4");
			__asm__ ("sta %v", cy1);

			// cy2 = (_eny + 15) >> 4;
			__asm__ ("lda %v", _eny);
			__asm__ ("ldx %v+1", _eny);
			__asm__ ("clc");
			__asm__ ("adc #15");
			__asm__ ("bcc %g", EHMCNN0CC);
			__asm__ ("inx");
		EHMCNN0CC:
			__asm__ ("jsr shrax4");
			__asm__ ("sta %v", cy2);

			// if (envx < 0)
			// To check if a number is NEGATIVE,
			// we just have to check if bit 7 is 1:
			__asm__ ("lda %v", envx);
			__asm__ ("asl a");				// bit 7 goes to C
			__asm__ ("bcc %g", EHMCNN0);	// Not negative.

				// envx < 0
				__asm__ ("lda #8");
				__asm__ ("sta %v", rdx);

				__asm__ ("lda %v", _enx);
				__asm__ ("lsr a");
				__asm__ ("lsr a");
				__asm__ ("lsr a");
				__asm__ ("lsr a");
				__asm__ ("sta %v", cx1);
				__asm__ ("sta %v", cx2);
				__asm__ ("jmp %g", EHMCNNE);

			EHMCNN0:
				// else {
				__asm__ ("lda #240");
				__asm__ ("sta %v", rdx);
				
				__asm__ ("lda %v", _enx);
				__asm__ ("clc");
				__asm__ ("adc #7");
				__asm__ ("lsr a");
				__asm__ ("lsr a");
				__asm__ ("lsr a");
				__asm__ ("lsr a");
				__asm__ ("sta %v", cx1);
				__asm__ ("sta %v", cx2);

		EHMCNNE:
			cm_two_points ();
			__asm__ ("ldy %v", enit);
			// if ((at1 & ENEMS_OBSTACLE_BITS) || (at2 & ENEMS_OBSTACLE_BITS) || _enx == rdx) {

			__asm__ ("lda %v", at1);
			__asm__ ("and #%b", ENEMS_OBSTACLE_BITS);
			__asm__ ("bne %g", EHMCCHD);

			__asm__ ("lda %v", at2);
			__asm__ ("and #%b", ENEMS_OBSTACLE_BITS);
			__asm__ ("bne %g", EHMCCHD);

			__asm__ ("lda %v", _enx);
			__asm__ ("cmp %v", rdx);
			__asm__ ("bne %g", EHMCNT7E);

		EHMCCHD:
			__asm__ ("lda #0");
			__asm__ ("sbc %v", _enmx);
			__asm__ ("sta %v", _enmx);

			__asm__ ("lda %v", encx);
			__asm__ ("sta %v", _enx);

			//__asm__ ("lda #1");
			//__asm__ ("sta %v", enhitbg);

	EHMCNT7E:

	// ==========================================================

		// Calculate relative position
		rdy = _eny - cam_pos;

		// Destroy?
		if (rdy >= 220) enems_destroy ();

	// ==========================================================

	// Pua hit

		gpit = PUAS_MAX; while (gpit) {	--gpit;
			if (puas_st [gpit] == 1) {
				if (CLB (puas_x [gpit], puas_y [gpit], _enx, _eny)) {
					if (c_level_killable_enems & pow2 [rdt]) {
						enems_kill ();
						bcd_score_add_5_2 (rdt, 0);
						pindex = puas_who [gpit];
						hud_update_score ();
						puas_st [gpit] = 0;
					} else puas_st [gpit] = 2;
					sfx_play (SFX_ENEMY_HIT, SC_ROOM);
				}
			}
		}

	// ==========================================================

		// paint

		if (rdt == 16) {
			// Explosion

			// logic
			if (_enct) _enct --; else {
				enems_destroy ();
				goto enems_do_loop;
			}

			// base
			rda = c_spr_base [0];
		} else {
			// base
			rda = c_spr_base [rdt];

			switch (rdt) {

				case 4:
					// facing
					if (prx < _enx) rda += EN_FACING;
					// frame
					if (rdd) rda += 2; else
					rda += (frame_counter >> 4) & 1;

					break;
				case 2:
				case 3:
					// facing
					if (_enmx < 0) rda += EN_FACING;
					// frame
					rda += (_enx >> 4) & 1;
					break;
				case 6:
					if (_ENSTATE == 0 || (_enct < 32 && half_life)) rda = 0;
					break;

				case 8:
					rda += ((frame_counter >> 2) & 3);
					break;
			}

			// collide with player
			#include "engine/enem_mods/enems_collision_2p.h"
		}

		if (rdy < 220 && rda) oam_index = oam_meta_spr (
			_enx, rdy + SPRITE_ADJUST, 
			oam_index,
			spr_enems [rda]
		);

		// Undo local temporary copy in ZP
		__asm__ ("ldy %v", enit);

		__asm__ ("lda %v", _enct);
		__asm__ ("sta %v, y", enct);
		
		__asm__ ("lda %v", _enx);
		__asm__ ("sta %v, y", enx);

		__asm__ ("lda %v", _enx1);
		__asm__ ("sta %v, y", enx1);

		__asm__ ("lda %v", _enx2);
		__asm__ ("sta %v, y", enx2);

		__asm__ ("lda %v", _enmx);
		__asm__ ("sta %v, y", enmx);

		__asm__ ("lda %v", _enmy);
		__asm__ ("sta %v, y", enmy);

		__asm__ ("tya");
		__asm__ ("asl a");
		__asm__ ("tax");

		__asm__ ("lda %v", _eny);
		__asm__ ("sta %v, x", eny);
		__asm__ ("lda %v+1", _eny);
		__asm__ ("sta %v+1, x", eny);


		__asm__ ("jmp %g", enems_do_loop);
	enems_do_loop_done:

	enstart ++; if (enstart == ENEMS_MAX) enstart = 0;
}
