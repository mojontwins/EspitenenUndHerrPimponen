// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// level set

const unsigned char * const level_map [] = { map_0, map_1, map_2, map_3, map_4, map_5, map_6, map_5, map_8, map_9 };
const unsigned char * const level_ts_tmaps_0 [] = { ts0_tmaps_0, ts1_tmaps_0, ts0_tmaps_0, ts3_tmaps_0, ts4_tmaps_0, ts1_tmaps_0, ts6_tmaps_0, ts7_tmaps_0, ts8_tmaps_0, ts9_tmaps_0 };
const unsigned char * const level_ts_tmaps_1 [] = { ts0_tmaps_1, ts1_tmaps_1, ts0_tmaps_1, ts3_tmaps_1, ts4_tmaps_1, ts1_tmaps_1, ts6_tmaps_1, ts7_tmaps_1, ts8_tmaps_1, ts9_tmaps_1 };
const unsigned char * const level_ts_tmaps_2 [] = { ts0_tmaps_2, ts1_tmaps_2, ts0_tmaps_2, ts3_tmaps_2, ts4_tmaps_2, ts1_tmaps_2, ts6_tmaps_2, ts7_tmaps_2, ts8_tmaps_2, ts9_tmaps_2 };
const unsigned char * const level_ts_tmaps_3 [] = { ts0_tmaps_3, ts1_tmaps_3, ts0_tmaps_3, ts3_tmaps_3, ts4_tmaps_3, ts1_tmaps_3, ts6_tmaps_3, ts7_tmaps_3, ts8_tmaps_3, ts9_tmaps_3 };
const unsigned char * const level_ts_pals_0 [] = { ts0_pals_0, ts1_pals_0, ts0_pals_0, ts3_pals_0, ts4_pals_0, ts1_pals_0, ts6_pals_0, ts7_pals_0, ts8_pals_0, ts9_pals_0 };
const unsigned char * const level_ts_pals_1 [] = { ts0_pals_1, ts1_pals_1, ts0_pals_1, ts3_pals_1, ts4_pals_1, ts1_pals_1, ts6_pals_1, ts7_pals_1, ts8_pals_1, ts9_pals_1 };
const unsigned char * const level_ts_pals_2 [] = { ts0_pals_2, ts1_pals_2, ts0_pals_2, ts3_pals_2, ts4_pals_2, ts1_pals_2, ts6_pals_2, ts7_pals_2, ts8_pals_2, ts9_pals_2 };
const unsigned char * const level_ts_pals_3 [] = { ts0_pals_3, ts1_pals_3, ts0_pals_3, ts3_pals_3, ts4_pals_3, ts1_pals_3, ts6_pals_3, ts7_pals_3, ts8_pals_3, ts9_pals_3 };

const unsigned char * const level_pal_bg [] = { palts0, palts1, palts2, palts3, palts4, palts5, palts6, palts7, palts8, palts9 };
//const unsigned char * const level_pal_fg [] = { palss0, palss0, palss0, palss0, palss0, palss0, palss0, palss0, palss0, palss0 };

const unsigned char * const level_behs [] = { behs0, behs1, behs0, behs3, behs4, behs1, behs6, behs7, behs4, behs0 };
const unsigned char * const level_strips [] = { enems_strip_0, enems_strip_1, enems_strip_2, enems_strip_3, enems_strip_4, enems_strip_5, enems_strip_6, enems_strip_5, enems_strip_8, enems_strip_9 };

const unsigned char * const * const level_spr_enems [] = { spr_enems0, spr_enems0, spr_enems1, spr_enems0, spr_enems0, spr_enems1, spr_enems0, spr_enems0, spr_enems1, spr_enems0 };

const unsigned char level_killable_enems [] = {
	// Level 0, byte = 00001100
	0x0c,
	// Level 1, byte = 00001100
	0x0c,
	// Level 2, byte = 00001100
	0x0c,
	// Level 3, byte = 00001100
	0x0c,
	// Level 4, byte = 00001100
	0x0c,
	// Level 5, byte = 00001100
	0x0c,
	// Level 6, byte = 00001100
	0x0c,
	// Level 8, byte = 00001100
	0x0c,
	// Level 9, byte = 00001100
	0x0c,
};

const unsigned char * const level_spr_base [] = {
	spr_base0, spr_base0, spr_base0, spr_base0, spr_base0, spr_base0, spr_base0, spr_base0, spr_base0, spr_base0
};
