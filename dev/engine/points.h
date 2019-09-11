// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// Points, displays a small sprite going upwards for 16 frames.

void points_init (void) {
	gpit = POINTS_MAX; while (gpit --) {
		points_fc [gpit] = 0; 
	}
	points_slots_index = 0;
}

void points_create (void) {
	// Cycle among slots, destroy oldest.
	points_slots_index = (points_slots_index + 1) & 3;
	points_x [points_slots_index] = rdx; 
	points_y [points_slots_index]= rdy; 
	points_fc [points_slots_index] = 32; 
	points_id [points_slots_index] = rdt;
}

void points_do (void) {
	gpit = POINTS_MAX; while (gpit --) if (points_fc [gpit]) {
		points_fc [gpit] --;
		if (0 == (frame_counter & 7)) points_y [gpit] --;
		oam_index = oam_meta_spr (
			points_x [gpit], points_y [gpit],
			oam_index,
			spr_points [points_id [gpit]]
		);
	}

}
