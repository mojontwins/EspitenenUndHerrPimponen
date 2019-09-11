// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// player.h - player functions

void player_init (void) {
	aprx [pindex] = pindex ? 192 : 64;
	apry [pindex] = (MAP_H-3)<<4;
	apx [pindex] = aprx [pindex] << FIX_BITS;
	apy [pindex] = apry [pindex] << FIX_BITS;
	apvx [pindex] = apvy [pindex] = 0;
	apfacing [pindex] = pindex ? CELL_FACING_LEFT : CELL_FACING_RIGHT;
	apj [pindex] = ap_pressingA [pindex] = 0;
	apgotten [pindex] = 0; 
	apkill [pindex] = 0;
	aplife [pindex] = PLAYER_LIFE_INI;
	apst [pindex] = EST_NORMAL; 
}

void players_init (void) {
	pindex = 0; player_init ();
	pindex = 1; player_init ();
}

void player_find_safe_spot (void) {
	// Finds a safe spot to spawn player 2
	pry = (cam_pos & 0xfff0) + 16;	// Start from the top of the viewport
	cy1 = LSB (pry);
	cx1 = pindex ? 15 : 0;
	
	while (1) {
		if (behs [SCREEN_BUFFER [cx1 | cy1]] & FLOOR_BITS) break;
		if (cx1) {
			if (pindex) -- cx1; else ++ cx1;
		} else { 
			cx1 = 15; pry += 16; cy1 += 16;
		}
	}

	aprx [pindex] = 4 + (cx1 << 4); apry [pindex] = pry - 16;
	apx [pindex] = aprx [pindex] << FIX_BITS;
	apy [pindex] = apry [pindex] << FIX_BITS;
	apon [pindex] = 1;	
}

void player_hit (void) {
	sfx_play (SFX_PLAYER_HIT, SC_PLAYER);

	if (rds) {
		pvx = ADD_SIGN (rds, PLAYER_VX_REBOUND);
	} else {
		pvx = ADD_SIGN (-pvx, PLAYER_VX_REBOUND);
	}
	
	if (plife) {
		pst = EST_FLICKER;
		pct = 100;
		plife --;
		hud_update_life ();
	} else {
		pkill = 1;
	}
}

void player_respawn (void) {
	-- aplife [pindex]; 
	player_find_safe_spot ();
	apvx [pindex] = apvy [pindex] = apkill [pindex] = 0;
	plife = aplife [pindex];
	hud_update_life ();	
}

void player_move (void) {
	if (pkill) {
#ifdef PDEAD_UPRISE
		psprid = CELL_DEAD;
		pst = EST_DEAD;
		pry --;
#else
		psprid = pfacing + CELL_DEAD;
		pst = EST_DEAD;
		pvy += PLAYER_G;
		if (pvy > PLAYER_VY_FALLING_MAX) pvy = PLAYER_VY_FALLING_MAX;
		py += pvy;
		pry = py >> FIX_BITS;
#endif
	} else 
#ifdef DEBUG
	if (debug_on) {
		pry -= 2; py = pry << FIX_BITS;
	} else
#endif
	{
		// Vertical axis
		// -------------

		if (!pgotten) {
			if (!pj) pvy += PLAYER_G; else pvy += PLAYER_G_JUMPING;
			if (pvy > PLAYER_VY_FALLING_MAX) pvy = PLAYER_VY_FALLING_MAX;
		} else if (!pj) pvy = 0;

		py += pvy;
		
		prx = px >> FIX_BITS;		// Pixel position
		pry = py >> FIX_BITS;
		cx1 = prx >> 4;				// Tile position of the corners of the bounding box
		cx2 = (prx + 7) >> 4;
		
		// Collision detection 
		// NOTE: spikes are only detected downwards. Add more detections if you need'em.
		if (pvy < 0) {
			cy1 = cy2 = pry >> 4;
			cm_two_points ();
			if ((at1 & OBSTACLE_BIT) || (at2 & OBSTACLE_BIT)) {
				pgotten = pvy = 0;
				pry = (cy1 + 1) << 4;
				py = pry << FIX_BITS;
			}
#ifdef PLAYER_SINKS
			else if (psinking) {
				cy1 = cy1 = (pry + 12) >> 4;
				cm_two_points (); 
				if ((at1 & SINK_BIT) || (at2 & SINK_BIT)) {
					if (pvy < -PLAYER_VY_EXIT_SINKING) pvy = -PLAYER_VY_EXIT_SINKING;
				} else psinking = 0;
			}
#endif			
		} else if (pvy > 0) {
			cy1 = cy2 = (pry + 15) >> 4;
			cm_two_points ();
			rdy = ((pry - 1) & 0xf);
			*((unsigned char *) 0xf1) = rdy;
#ifdef PLAYER_EVIL_TILE
			rda = pry & 0xf;
#endif
			if (rdy < 6 && ((at1 & FLOOR_BITS) || (at2 & FLOOR_BITS))) {
				pgotten = pvy = 0;
				pry = (cy2 - 1) << 4;
				py = pry << FIX_BITS;
			}
#ifdef PLAYER_EVIL_TILE 
			else if (((at1 & 1) || (at2 & 1)) && rda && rda < 6 && !ppossee) {
				pvy = -PLAYER_VY_JUMP_INITIAL<<1;
				pkill = 1;
			}
#endif
#ifdef PLAYER_SINKS
			else {
				if ((at1 & SINK_BIT) || (at2 & SINK_BIT)) {
#if PLAYER_VY_SINKING==0xff
					pvy = half_life;
#else					
					pvy = PLAYER_VY_SINKING;
#endif					
					psinking = 1;
				} else psinking = 0; 
			}
#endif	
		}	
		
		// Are we right over a platform / solid?
		cy1 = cy2 = (pry + 16) >> 4;
		cm_two_points ();
		ppossee = ((pry & 0xf) == 0) && ((at1 & FLOOR_BITS) || (at2 & FLOOR_BITS));	
#ifdef PLAYER_SLIPPERY
		pslips = ppossee && ((at1 & PLAYER_SLIPPERY_BIT) || (at2 & PLAYER_SLIPPERY_BIT));
#endif

		// Jump
		if (pad & PAD_A) {
			if (!p_pressingA) {
				p_pressingA = 1;
				if (!pj) {
					if (pgotten || ppossee || psinking) {
						pj = 1; pctj = 0; 
						pvy = -PLAYER_VY_JUMP_INITIAL;
						sfx_play (SFX_JUMP, SC_PLAYER);
						//if (ppossee) player_register_safe_spot ();
					}
				} 
			}
			if (pj) {
				++ pctj; if (pctj == PLAYER_VY_JUMP_A_STEPS) pj = 0;
			}
		} else {
			p_pressingA = 0;
			if (pj) {
				if (pvy < -PLAYER_VY_JUMP_RELEASE) pvy = -PLAYER_VY_JUMP_RELEASE;
				pj = 0;
			}
		}

		// Horizontal axis
		// ---------------

#ifdef PLAYER_SLIPPERY
		if (pslips) {
			_PLAYER_AX = PLAYER_AX_SLIPPERY;
			//_PLAYER_RX = half_life ? PLAYER_RX_SLIPPERY : 0;
			_PLAYER_RX = half_life;
		} else {
			_PLAYER_AX = PLAYER_AX;
			_PLAYER_RX = PLAYER_RX;
		}
#endif
		
#ifdef PLAYER_HAS_PUNCH
		pdpadaccelerates = (!ppunching && ppossee) || !ppossee;
		pndpaddeccelerates = !(ppunching && !ppossee);
#endif

		if ((pad & PAD_LEFT)
#ifdef PLAYER_HAS_PUNCH
		&& pdpadaccelerates
#endif
		) {
			pfacing = CELL_FACING_LEFT;
			pvx -= _PLAYER_AX; 
			if (pvx < -PLAYER_VX_MAX) pvx = -PLAYER_VX_MAX;
		} else if ((pad & PAD_RIGHT)
#ifdef PLAYER_HAS_PUNCH
		&& pdpadaccelerates
#endif
		) {
			pfacing = CELL_FACING_RIGHT;
			pvx += _PLAYER_AX;
			if (pvx > PLAYER_VX_MAX) pvx = PLAYER_VX_MAX;
		} else 
#ifdef PLAYER_HAS_PUNCH
		if (pndpaddeccelerates)
#endif 
		{
			if (pvx > 0) {
				pvx -= _PLAYER_RX;
				if (pvx < 0) pvx = 0;
			} else if (pvx < 0) {
				pvx += _PLAYER_RX;
				if (pvx > 0) pvx = 0;
			}
		}
		
		px += pvx;
		if (pgotten) px += ptgmx; else ptgmx = 0;
		if (px < (4<<FIX_BITS)) {px = 4<<FIX_BITS; pvx = 0;}
		if (px > (244<<FIX_BITS)) {px = 244<<FIX_BITS; pvx = 0;}

		prx = px >> FIX_BITS;		// Pixel position
		cy1 = pry >> 4;
		cy2 = (pry + 15) >> 4;

		if (pvx + ptgmx < 0) {
			cx1 = cx2 = prx >> 4;
			cm_two_points ();
			if ((at1 & OBSTACLE_BIT) || (at2 & OBSTACLE_BIT)) {
				pvx = 0;
				prx = (cx1 + 1) << 4;
				px = prx << FIX_BITS;
			}
		} else if (pvx + ptgmx > 0) {
			cx1 = cx2 = (prx + 7) >> 4;
			cm_two_points ();
			if ((at1 & OBSTACLE_BIT) || (at2 & OBSTACLE_BIT)) {
				pvx = 0;
				prx = ((cx1 - 1) << 4) + 8;
				px = prx << FIX_BITS;
			}
		}

		// Tile detection stuff
		if (pry > 8 + cam_pos) {
			cx1 = cx2 = (prx + 4) >> 4;
			cy1 = cy2 = (pry + 7) >> 4;
			cm_two_points ();
			rdd = SCREEN_BUFFER [cx1 | ((cy1 & 15) << 4)];

#ifdef PLAYER_TILE_GET
			#include "engine/player_mods/player_tile_get.h"
#endif		
		}

		// Action button stuff
		if (pad & PAD_B) {
			if (!p_pressingB) {
				p_pressingB = 1;
#ifdef PLAYER_HAS_PUNCH
				if (!ppunching) ppunching = 16;
#endif

#ifdef PLAYER_PUAS
				sfx_play (SFX_PUAS, SC_PLAYER);
				puas_create ();
#endif
			}
		} else p_pressingB = 0;

#ifdef PLAYER_HAS_PUNCH
		#include "engine/player_mods/player_punch.h"
#endif	
		
		// This is much needed here
		if (pry < cam_pos && !autoscroll) { pry = cam_pos +1; py = pry << FIX_BITS; }

		// Kill player?
		if ((ntsc && pry > 220 + cam_pos) || (pry > 228 + cam_pos)) {
			pkill = 2;
			sfx_play (SFX_PLAYER_HIT, SC_PLAYER);
#ifndef PDEAD_UPRISE
			pvy = -PLAYER_VY_JUMP_INITIAL<<1;
			pry -= 8;
			py = pry << FIX_BITS;
#endif
		}
		
		// Frame & sprite pos
		/*if (ppunching) {
			psprid = CELL_PUNCHING;
		} else */if ((ppossee || pgotten || psinking) && pvy >= 0) {
			if (ABS (pvx) > PLAYER_VX_MIN) {
				psprid = CELL_WALK_BASE + ((prx >> 4) & 3);
			} else psprid = CELL_IDLE;
		} else psprid = CELL_AIRBORNE;
		
		psprid += pfacing;		

		if (pst == EST_FLICKER) {
			if (pct) pct --; else pst = EST_NORMAL;
		}
	}

	if (pry <= cam_pos) return;
	
	if (pst != EST_FLICKER || half_life) oam_index = oam_meta_spr (
		prx, pry - cam_pos + SPRITE_ADJUST, 
		oam_index, 
		spr_player [pcharacter][psprid]
	);
}

void players_move (void) {
		__asm__ ("ldy #0");

	players_move_loop:
		__asm__ ("lda %v, y", apon);
		__asm__ ("beq %g", players_move_continue);

		// Copy values from arrays
		__asm__ ("lda %v, y", apvx);
		__asm__ ("sta %v", pvx);
		__asm__ ("lda %v, y", apvy);
		__asm__ ("sta %v", pvy);

		__asm__ ("lda %v, y", aprx);
		__asm__ ("sta %v", prx);

		__asm__ ("lda %v, y", apfacing);
		__asm__ ("sta %v", pfacing);
		__asm__ ("lda %v, y", apkill);
		__asm__ ("sta %v", pkill);

		__asm__ ("lda %v, y", aptgmx);
		__asm__ ("sta %v", ptgmx);

		__asm__ ("lda %v, y", aplives);
		__asm__ ("sta %v", plives);
		__asm__ ("lda %v, y", aplife);		
		__asm__ ("sta %v", plife);

		__asm__ ("lda %v, y", apst);
		__asm__ ("sta %v", pst);
		__asm__ ("lda %v, y", apct);
		__asm__ ("sta %v", pct);

		__asm__ ("lda %v, y", apsinking);
		__asm__ ("sta %v", psinking);

		__asm__ ("lda %v, y", ap_pressingA);
		__asm__ ("sta %v", p_pressingA);
		__asm__ ("lda %v, y", ap_pressingB);
		__asm__ ("sta %v", p_pressingB);		

		__asm__ ("lda %v, y", apj);
		__asm__ ("sta %v", pj);
		__asm__ ("lda %v, y", apctj);
		__asm__ ("sta %v", pctj);
		__asm__ ("lda %v, y", appossee);
		__asm__ ("sta %v", ppossee);
		__asm__ ("lda %v, y", apgotten);
		__asm__ ("sta %v", pgotten);

		__asm__ ("lda %v, y", apcharacter);
		__asm__ ("sta %v", pcharacter);

		__asm__ ("tya");
		__asm__ ("asl a");
		__asm__ ("tax");

		__asm__ ("lda %v, x", apy);
		__asm__ ("sta %v, x", apcy);
		__asm__ ("sta %v", py);

		__asm__ ("lda %v+1, x", apy);
		__asm__ ("sta %v+1, x", apcy);
		__asm__ ("sta %v+1", py);

		__asm__ ("lda %v, x", apx);
		__asm__ ("sta %v, x", apcx);
		__asm__ ("sta %v", px);
		__asm__ ("lda %v+1, x", apx);
		__asm__ ("sta %v+1, x", apcx);
		__asm__ ("sta %v+1", px);

		__asm__ ("lda %v, x", apry);
		__asm__ ("sta %v", pry);
		__asm__ ("lda %v+1, x", apry);
		__asm__ ("sta %v+1", pry);

		// Save index
		__asm__ ("sty %v", pindex);

		// assign pad
		if (pindex == 0) pad = pad0; else pad = pad1;
		
		// Call routine
		player_move ();

		// Retrieve index
		__asm__ ("ldy %v", pindex);

		// Copy values to arrays
		__asm__ ("lda %v", pvx);
		__asm__ ("sta %v, y", apvx);
		__asm__ ("lda %v", pvy);
		__asm__ ("sta %v, y", apvy);

		__asm__ ("lda %v", prx);
		__asm__ ("sta %v, y", aprx);

		__asm__ ("lda %v", pfacing);
		__asm__ ("sta %v, y", apfacing);
		__asm__ ("lda %v", pkill);
		__asm__ ("sta %v, y", apkill);

		__asm__ ("lda %v", ptgmx);
		__asm__ ("sta %v, y", aptgmx);

		__asm__ ("lda %v", plives);
		__asm__ ("sta %v, y", aplives);
		__asm__ ("lda %v", plife);
		__asm__ ("sta %v, y", aplife);		

		__asm__ ("lda %v", pst);
		__asm__ ("sta %v, y", apst);
		__asm__ ("lda %v", pct);
		__asm__ ("sta %v, y", apct);

		__asm__ ("lda %v", psinking);
		__asm__ ("sta %v, y", apsinking);

		__asm__ ("lda %v", p_pressingA);
		__asm__ ("sta %v, y", ap_pressingA);
		__asm__ ("lda %v", p_pressingB);		
		__asm__ ("sta %v, y", ap_pressingB);

		__asm__ ("lda %v", pj);
		__asm__ ("sta %v, y", apj);
		__asm__ ("lda %v", pctj);
		__asm__ ("sta %v, y", apctj);
		__asm__ ("lda %v", ppossee);
		__asm__ ("sta %v, y", appossee);
		__asm__ ("lda %v", pgotten);
		__asm__ ("sta %v, y", apgotten);

		__asm__ ("tya");
		__asm__ ("asl a");
		__asm__ ("tax");

		__asm__ ("lda %v", py);
		__asm__ ("sta %v, x", apy);
		__asm__ ("lda %v+1", py);
		__asm__ ("sta %v+1, x", apy);

		__asm__ ("lda %v", px);
		__asm__ ("sta %v, x", apx);
		__asm__ ("lda %v+1", px);
		__asm__ ("sta %v+1, x", apx);

		__asm__ ("lda %v", pry);
		__asm__ ("sta %v, x", apry);
		__asm__ ("lda %v+1", pry);
		__asm__ ("sta %v+1, x", apry);

	players_move_continue:
		__asm__ ("iny");
		__asm__ ("cpy #2");
		__asm__ ("bne %g", players_move_loop);

	if (collide_players) {
		if (
			aprx [0] + 7 >= aprx [1] && 
			aprx [1] + 7 >= aprx [0] && 
			apry [0] + 15 >= apry [1] && 
			apry [1] + 15 >= apry [0]
		) {
			rds = apvx [0];
			apvx [0] = apvx [1];
			apvx [1] = rds;

			rds = apvy [0];
			apvy [0] = apvy [1];
			apvy [1] = rds;

			apx [0] = apcx [0];
			apx [1] = apcx [1];
			apy [0] = apcy [0];
			apy [1] = apcy [1];
		}
	}
}
