// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

void enems_do (void) {

	aptgmx [0] = apgotten [0] = 
	aptgmx [1] = apgotten [1] = 0;

	enit = enstart;
	enjt = ENEMS_MAX; while (enjt --) {

		enit += (ENEMS_MAX-1); if (enit >= ENEMS_MAX) enit -= ENEMS_MAX;
		
		if (ena [enit]) {

			// Local temporary copy in ZP
			_enct = enct [enit];
			_enx = enx [enit]; _eny = eny [enit];
			_enx1 = enx1 [enit]; _enx2 = enx2 [enit];
			_enmx = enmx [enit]; _enmy = enmy [enit];
			
			// Blah
			rdt = ent [enit];

			envx = envy = 0;

#ifndef ENEM_DISABLE_BULLETS
			// Previous stuff: based on type: shoot, fall, pursue.
			if (rdt == 1 || rdt == 9) {
				// Shoot
				if (rand8 () < _ENFREQ) {
					rdx = _enx; rdy = _eny + 4;
					bullets_shoot_player ();
				}
			}
#endif

			switch (rdt) {
#ifndef ENEM_DISABLE_THROWABLES
				case 5:
					// arrows fsm
					if (_ENSTATE != 2) {
						if (_enct) _enct --; else {
							if (_ENSTATE < 3) {
								_ENSTATE ++;
								_enct = _ENMAXCT;
							} else {
								_ENSTATE = 0;
								_enct = _ENMAXCT;
								_eny = eny1 [enit];
							}
						}
						break;
					}
					// Correct! no break here. If state = 2, fall
					// Sorry if this is hard to follow :*)
#endif
				case 3:
					// Gravity
					envy = enems_lut_falling [_enmy];
					if (_enmy < ENEMS_LUT_FALLING_MAXIDX) _enmy ++;
					// Correct! no break here. Move left/right
				case 2:
					if (_enx1 || half_life ) envx = _enmx;
					break;

				case 7:
					envx = _enmx;
					break;	
				case 4:
					rdd = 1; 

					// Retreaters
					if (eny1 [enit] == 0) {
						_eny += 2; break;
					} else if ( eny1 [enit] > _eny + 240 ) {
						eny1 [enit] = 0;
					}

					// Pursue
					// Pursue WHOM?
					if (apon [0] && apon [1]) {
						pindex = enit & 1;
					} else if (apon [0]) {
						pindex = 0;
					} else if (apon [1]) {
						pindex = 1;
					}

					prx = aprx [pindex];
					pry = apry [pindex];
					pfacing = apfacing [pindex];

					if ((pfacing && prx > _enx) ||
						(!pfacing && prx < _enx)) {
						break;
					}

					rdd = 0;

					if ((enit + half_life) & 1) {
						break;
					}

					rda = rand8 ();

					if (rda > 2) {
						if (prx > _enx) {
							if (_enmx < ENEMS_LUT_ACCELERATION_MAXIDX) ++ _enmx;
						}
						if (prx < _enx) {
							if (_enmx > -ENEMS_LUT_ACCELERATION_MAXIDX) -- _enmx;
						}
					}

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

					break;

				case 6:
					if (_enct) _enct --; else {
						_ENSTATE = !_ENSTATE;
						_enct = _ENSTATE ? 128 : 64;
					}
					break;

				case 9:
					envx = _enmx; envy = _enmy;
					break;
			}
			
			// Vertical movement & collision
			_eny += envy;

			enhitbg = 0;
			cx1 = _enx >> 4;
			cx2 = (_enx + 7) >> 4;
			if (envy < 0) {
				cy2 = cy1 = (_eny) >> 4;
				cm_two_points ();
				if ((at1 & ENEMS_OBSTACLE_BITS) || (at2 & ENEMS_OBSTACLE_BITS)) {
					_enmy = 0;
					_eny = (cy1 + 1) << 4;
					enhitbg = 1;
				}
			} else if (envy > 0) {
				cy2 = cy1 = (_eny + 15) >> 4;
				cm_two_points ();
				if ((at1 & ENEMS_FLOOR_BITS) || (at2 & ENEMS_FLOOR_BITS)) {
					_enmy = (rdt == 9) ? -_enmy : 0;
					_eny = (cy2 - 1) << 4;
					enhitbg = 1;					
				}
			}

			// Horizontal movement & collision
			encx = _enx;
			_enx += envx;

			if (rdt == 7) {
				if (_enx < _enx1 || _enx > _enx2) {
					_enmx = -_enmx;
					_enx = encx;
				}
			} else if (envx) {
				cy1 = _eny >> 4;
				cy2 = (_eny + 15) >> 4;
				if (envx < 0) {
					rdx = 8;
					// cx2 = cx1 = rdt == 7 ? (_enx - 12) >> 4 : _enx >> 4;
					cx2 = cx1 = (_enx >> 4);
				} else {
					rdx = 240;
					//cx2 = cx1 = rdt == 7 ? (_enx + 19) >> 4 : (_enx + 7) >> 4;
					cx2 = cx1 = (_enx + 7) >> 4;
				}
				cm_two_points ();
				if ((at1 & ENEMS_OBSTACLE_BITS) || (at2 & ENEMS_OBSTACLE_BITS) || _enx == rdx) {
					_enmx = -_enmx;
					_enx = encx;
					enhitbg = 1;
				}
			}

#ifndef ENEM_DISABLE_THROWABLES
			// State advance on BG collision
			if (enhitbg) {
				_ENSTATE ++;
				switch (rdt) {
					case 5:
						_enct = drop_fixed_times [3];
				}				
			}
#endif

			// relative position
			rdy = _eny - cam_pos;

			// destroy
			if (rdy >= 220) {
#ifndef ENEM_DISABLE_THROWABLES
				if (rdt != 5) 
#endif
					enems_destroy ();
			}

#ifndef ENEM_DISABLE_THROWABLES			
			if (eny1 [enit] > cam_pos + 220) enems_destroy ();
#endif

			// Pua hit
#ifdef PLAYER_PUAS
			gpit = PUAS_MAX; while (gpit --) if (puas_st [gpit] == 1) {
				if (CLB (puas_x [gpit], puas_y [gpit], _enx, _eny)) {
					if (c_level_killable_enems & pow2 [rdt]) {
						enems_kill ();
						bcd_score_add_5_2 (rdt, 0);
						hud_update_score ();
						puas_st [gpit] = 0;
					} else puas_st [gpit] = 2;
					sfx_play (SFX_ENEMY_HIT, SC_ROOM);
				}
			}
#endif
			
			// paint

			if (rdt == 16) {
				// Explosion

				// logic
				if (_enct) _enct --; else {
					enems_destroy ();
					continue;
				}

				// base
				rda = c_spr_base [0];
			} else {
				// base
				rda = c_spr_base [rdt];

				switch (rdt) {
#ifndef ENEM_DISABLE_BULLETS
					case 1:
#endif
					case 4:
						// facing
						if (prx < _enx) rda += EN_FACING;
						// frame
#ifndef ENEM_DISABLE_BULLETS
						if (rdt == 4 && rdd) rda += 2; else
#else
						if (rdd) rda += 2; else
#endif
						rda += (frame_counter >> 4) & 1;

						break;
					case 2:
					case 3:
						// facing
						if (_enmx < 0) rda += EN_FACING;
						// frame
						rda += (_enx >> 4) & 1;
						break;
#ifndef ENEM_DISABLE_THROWABLES
					case 5:
						if (_ENSTATE == 3) rda += (_enct >> 2) + 1;
						break;
#endif
					case 6:
						if (_ENSTATE == 0 || (_enct < 32 && half_life)) rda = 0;
						break;

					case 8:
						rda += ((frame_counter >> 2) & 3);
						break;
				}

				// collide with player
				//#include "engine/enem_mods/enems_collision.h"
				#include "engine/enem_mods/enems_collision_2p.h"
			}

			if (rdy < 220 && rda) oam_index = oam_meta_spr (
				_enx, rdy + SPRITE_ADJUST, 
				oam_index,
				spr_enems [rda]
			);

			// Undo local temporary copy in ZP
			enct [enit] = _enct;
			enx [enit] = _enx; eny [enit] = _eny;
			enx1 [enit] = _enx1; enx2 [enit] = _enx2;
			enmx [enit] = _enmx; enmy [enit] = _enmy;
		}
	}
	enstart ++; if (enstart == ENEMS_MAX) enstart = 0;
}
