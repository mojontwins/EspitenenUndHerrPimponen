// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// player.h - player functions

	if (!pkill) switch (rdt) {
		case 6:
			if (_ENSTATE) {
				if (prx + 11 >= _enx && prx <= _enx + 11) {
					if (pry + 16 >= _eny && pry + 12 <= _eny) {
						player_gotten ();
					}
				}
			}
			break;
		case 7:
			if (prx + 19 >= _enx && prx <= _enx + 19) {
				if (pry + 16 >= _eny && pry + 12 <= _eny) {
					player_gotten ();
				}
			}
			break;
		default:		
			// Punch first, then body
			/*if (ppunchx + 7 >= _enx && ppunchx <= _enx + 7 &&
				ppunchy + 7 >= _eny && ppunchy <= _eny + 15 &&
				ppunching > 8) {
				if (c_level_killable_enems & pow2 [rdt]) enems_kill ();
			} else */
			if (
				CLE (prx, pry, _enx, _eny) && pst != EST_FLICKER
				#ifdef DEBUG
					&& !debug_on
				#endif
			) {
				rds = envx;
				// Custom ana
				if (rdt == 1) goal_time = 0; else
				// if (rdt == 8) player_replenish (); else 
				player_hit ();

				#ifndef ENEM_DISABLE_THROWABLES
					if (rdt != 5 && rdt != 1) 
				#endif
				enems_kill ();
			}
			break;
	}