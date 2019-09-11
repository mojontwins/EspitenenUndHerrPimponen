// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

if (at1 & PLAYER_TILE_GET_BIT) {
	// Delete from screen
	rdb = ((cy1 - (map_tilepos & 0x0f)) & 0x0f);	// Distance from top
	rdx = cx1 + cx1;								// x position in nametables
	rdy = (rdb + rdb) + (wtp & 0xfe);				// y position in nametables
	if (rdy >= 30) rdy -= 30;						// Crude mod 30
	rdt = 0;										// Erase with tile 0
	put_tile_ul ();									// Draw tile
	
	// Delete from buffer
	SCREEN_BUFFER [cx1 + ((cy1 & 15) << 4)] = 0;

	// Points
	rdt = rdd - 24;
	bcd_score_add_5_2 (pc_scores_a [rdt], pc_scores_b [rdt]);
	
	#ifdef POINTS_ENABLE
		rdy = (cy1 << 4) - cam_pos - 8 + SPRITE_ADJUST;
		rdx = cx1 << 4;
		points_create ();
	#endif

	hud_update_score ();

	// sfx
	sfx_play (sfx_item [rdt], SC_ROOM);
}

