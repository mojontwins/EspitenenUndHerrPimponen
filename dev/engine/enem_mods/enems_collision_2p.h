// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// player.h - player functions

for (pindex = 0; pindex < 2; ++ pindex) {
	if (apon [pindex]) {

		// Copy values from arrays

		__asm__ ("ldy %v", pindex);

		__asm__ ("lda %v, y", aprx);
		__asm__ ("sta %v", prx);
		__asm__ ("lda %v, y", apvx);
		__asm__ ("sta %v", pvx);

		__asm__ ("lda %v, y", apkill);
		__asm__ ("sta %v", pkill);
		__asm__ ("lda %v, y", aplife);
		__asm__ ("sta %v", plife);

		__asm__ ("lda %v, y", apst);
		__asm__ ("sta %v", pst);
		__asm__ ("lda %v, y", apct);
		__asm__ ("sta %v", pct);

		__asm__ ("lda %v, y", apgotten);
		__asm__ ("sta %v", pgotten);
		__asm__ ("lda %v, y", aptgmx);
		__asm__ ("sta %v", ptgmx);
	
		pry = apry [pindex];
		py = apy [pindex];
		
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
				if (
					CLE (prx, pry, _enx, _eny) && pst != EST_FLICKER
					#ifdef DEBUG
						&& !debug_on
					#endif
				) {
					rds = envx;
					// Custom ana
					if (rdt == 1) goal_time = 0; else
					//if (rdt == 8) player_replenish (); else 
					// End of custom
					player_hit ();
					#ifndef ENEM_DISABLE_THROWABLES
						if (rdt != 5 && rdt != 1) 
					#endif
					enems_kill ();
				}
				break;
		}

		// Save values to arrays

		__asm__ ("ldy %v", pindex);

		__asm__ ("lda %v", prx);
		__asm__ ("sta %v, y", aprx);
		__asm__ ("lda %v", pvx);
		__asm__ ("sta %v, y", apvx);

		__asm__ ("lda %v", pkill);
		__asm__ ("sta %v, y", apkill);
		__asm__ ("lda %v", plife);
		__asm__ ("sta %v, y", aplife);

		__asm__ ("lda %v", pst);
		__asm__ ("sta %v, y", apst);
		__asm__ ("lda %v", pct);
		__asm__ ("sta %v, y", apct);

		__asm__ ("lda %v", pgotten);
		__asm__ ("sta %v, y", apgotten);
		__asm__ ("lda %v", ptgmx);
		__asm__ ("sta %v, y", aptgmx);
	
		apry [pindex] = pry;
		apy [pindex] = py;
	}
}
