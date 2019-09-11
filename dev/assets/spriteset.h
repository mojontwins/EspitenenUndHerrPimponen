// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

const unsigned char * const spr_player0 [] = {
	// Facing right                                                 0
	sspl_01_a,										// Standing	    0
	sspl_01_a, sspl_02_a, sspl_01_a, sspl_00_a, 	// Walk cycle   1
	sspl_00_a,										// Jump         5
	sspl_02_a, 										// Falling      6
	sspl_03_a,										// Hit          7

	// Facing left                                                  8
	sspl_01_b,										// Standing	    0
	sspl_01_b, sspl_02_b, sspl_01_b, sspl_00_b, 	// Walk cycle   1
	sspl_00_b,										// Jump         5
	sspl_02_b, 										// Falling      6
	sspl_03_b,										// Hit          7
};

const unsigned char * const spr_player1 [] = {
	// Facing right                                                 0
	sspl_05_a,										// Standing	    0
	sspl_05_a, sspl_06_a, sspl_05_a, sspl_04_a, 	// Walk cycle   1
	sspl_04_a,										// Jump         5
	sspl_06_a, 										// Falling      6
	sspl_07_a,										// Hit          7

	// Facing left                                                  8
	sspl_05_b,										// Standing	    0
	sspl_05_b, sspl_06_b, sspl_05_b, sspl_04_b, 	// Walk cycle   1
	sspl_04_b,										// Jump         5
	sspl_06_b, 										// Falling      6
	sspl_07_b,										// Hit          7
};

const unsigned char * const * const spr_player [] = {
	spr_player0, spr_player1
};

const unsigned char * const spr_enems0 [] = {
	// First value is "don't paint sprite" in the engine, so
	0,
	// Type 2 (1)
	ssenb_00_a, ssenb_01_a, 0, 0, ssenb_00_b, ssenb_01_b, 0, 0,
	// Type 3 (9)
	ssena_00_a, ssena_01_a, 0, 0, ssena_00_b, ssena_01_b, 0, 0,
	// Type 4 (17)
	ssend_00_a, ssend_01_a, ssend_02_a, 0, ssend_00_b, ssend_01_b, ssend_02_b, 0,
	// Explosion (25)
	ssexpl,
	// Plataforma móvil (26)
	ssenc,
	// Plataforma flipflop (27)
	ssene,
	// Ana (28)
	ssana_a, 0, 0, 0, ssana_b, 0, 0, 0
};

const unsigned char * const spr_enems1 [] = {
	// First value is "don't paint sprite" in the engine, so
	0,
	// Type 2 (1)
	ssenb_00_a, ssenb_01_a, 0, 0, ssenb_00_b, ssenb_01_b, 0, 0,
	// Type 3 (9)
	ssenf_00_a, ssenf_01_a, 0, 0, ssenf_00_b, ssenf_01_b, 0, 0,
	// Type 4 (17)
	ssenf_02_a, ssenf_03_a, ssenf_04_a, 0, ssenf_02_b, ssenf_03_b, ssenf_04_b, 0,
	// Explosion (25)
	ssexpl,
	// Plataforma móvil (26)
	ssenc,
	// Plataforma flipflop (27)
	ssene,
	// Ana (28)
	ssana_a, 0, 0, 0, ssana_b, 0, 0, 0
};

// Base sprite for each enemy type 0..7
const unsigned char spr_base0 [] = {
	// Explosion, type 1, type 2..., type 7
//  0   1  2  3  4   5  6   7
	25, 28, 1, 9, 17, 0, 27, 26
};

// Points
const unsigned char * const spr_points [] = {
	sspointsa_00, sspointsa_01, sspointsa_02, sspointsa_03, 
	sspointsa_04, sspointsb_00, sspointsb_01, sspointsb_02
};

// Big metasprite with the hud!
const unsigned char spr_hud [] = {
	0, 0, 5, 3, 8, 0, 5, 3, 16, 0, 5, 3, 24, 0, 5, 3, 32, 0, 5, 3, 40, 0, 5, 3, 	// Six zeroes
	0, 8, 3, 0, 8, 8, 4, 3, 16, 8, 5, 3, 24, 8, 16, 1, 32, 8, 16, 1, 40, 8, 16, 1,	// heart X lives life life life
	128
};
