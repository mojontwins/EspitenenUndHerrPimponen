// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

void puas_init (void) {
	gpit = PUAS_MAX; while (gpit --) puas_st [gpit] = 0;
}

void puas_create (void) {
	gpit = PUAS_MAX; while (gpit --) {
		if (puas_st [gpit] == 0) {
			puas_y [gpit] = pry;
			puas_x [gpit] = prx;
			puas_mx [gpit] = pfacing ? PUAS_VX : -PUAS_VX;
			puas_st [gpit] = 1;
			puas_who [gpit] = pindex;
			break;
		}
	}
}

void puas_do (void) {
	gpit = PUAS_MAX; while (gpit --) {
		if (puas_st [gpit]) {
			_puas_y = puas_y [gpit];
			_puas_mx = puas_mx [gpit];
			if (puas_st [gpit] == 1) {
				_puas_x = puas_x [gpit] + _puas_mx;
			} else {
				_puas_y += 8;
			}
			if (_puas_x < PUAS_VX || _puas_x > 248 - PUAS_VX || _puas_y >= cam_pos + 232 ) {
				puas_st [gpit] = 0;				
			} else {
				if (_puas_y > cam_pos)	oam_index = oam_spr (
					_puas_x, _puas_y + SPRITE_ADJUST - cam_pos,
					PUAS_PATTERN,
					_puas_mx > 0 ? 0 : 0x40,
					oam_index
				);
				puas_x [gpit] = _puas_x;
				puas_y [gpit] = _puas_y;
			}
		}
	}
}
