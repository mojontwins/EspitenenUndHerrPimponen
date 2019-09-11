// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// Bullets!

void bullets_init (void) {
	bit = BULLETS_MAX; while (bit --) {
		ba [bit] = 0; bslots [bit] = bit;
	}
	bsloti = BULLETS_MAX;
}

void bullets_destroy (void) {
	ba [bit] = 0;
	bslots [bsloti] = bit;
	bsloti ++;
}

void bullets_do (void) {
	// Move and paint only odd / even bullets
	for (bit = half_life; bit < BULLETS_MAX; bit += 2) {
		if (ba [bit]) {
			// move
			bx [bit] += bvx [bit]; _bx = bx [bit];
			by [bit] += bvy [bit]; _by = by [bit];

			// bounds
			bpy = _by >> FIX_BITS;
			if (_bx < 0 || _bx > 256<<FIX_BITS ||
				bpy < cam_pos + 8 || bpy > cam_pos + 224) {
				bullets_destroy ();
				continue;
			}

			rdx = _bx >> FIX_BITS;
			rdy = bpy - cam_pos;

			// paint
			oam_index = oam_spr (rdx, rdy - 1, BULLETS_PATTERN, BULLETS_PALETTE, oam_index);

			// collide w/player
			if (pst == EST_NORMAL && CLB (rdx, bpy, prx, pry)) {
				rds = bvx [bit];
				player_hit ();
				bullets_destroy ();
			}
		}
	}
}

void bullets_shoot_player (void) {
	// Distance
	if (bsloti) {
		bsloti --;
		bit = bslots [bsloti];

		ba [bit] = 1;

		// Calculate distance
		rda = DELTA (prx, rdx); // dx
		rdb = DELTA (pry, rdy); // dy
		rdc = MIN (rda, rdb);	// MIN (dx, dy)
		rdd = rda + rdb - (rdc >> 1) - (rdc >> 2) + (rdc >> 4);

		bx [bit] = rdx << FIX_BITS;
		by [bit] = rdy << FIX_BITS;

		// Apply formula. Looks awkward but it's optimized for space and shitty compiler
		gpiit = BULLETS_V * rda / rdd; bvx [bit] = ADD_SIGN2 (px, bx [bit], gpiit);
		gpiit = BULLETS_V * rdb / rdd; bvy [bit] = ADD_SIGN2 (py, by [bit], gpiit);
	}
}

