// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// Precalculated stuff

const unsigned char attrs_adds [] = { 
	0xc0, 0xc8, 0xd0, 0xd8, 0xe0, 0xe8, 0xf0, 0xf8 
};

const unsigned char attr_buff_adds [] = {
	0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38
};

const unsigned char wp_addr_lo [] = {
	0x00, 0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0, 0xE0,
	0x00, 0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0, 0xE0,
	0x00, 0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0, 0xE0,
	0x00, 0x20, 0x40, 0x60, 0x80, 0xA0 //, 0xC0, 0xE0,
};

const unsigned char wp_addr_hi [] = {
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21,
	0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
	0x23, 0x23, 0x23, 0x23, 0x23, 0x23 //, 0x23, 0x23
};

const unsigned char shr2 [] = {
	0, 0, 0, 0, 1, 1, 1, 1,
	2, 2, 2, 2, 3, 3, 3, 3,
	4, 4, 4, 4, 5, 5, 5, 5,
	6, 6, 6, 6, 7, 7, 7, 7
};

#define ENEMS_LUT_FALLING_MAXIDX 15
const signed char enems_lut_falling [] = {
	0, 0, 1, 1, 1, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6
};

#define ENEMS_LUT_ACCELERATION_MAXIDX 15
const signed char enems_lut_acceleration [] = {
	0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1
};

const unsigned char drop_fixed_times [] = {
	0, 16, 0, 11
};

const unsigned char pow2 [] = { 1, 2, 4, 8, 16, 32, 64, 128 };

// Points for BCD
const unsigned char pc_scores_a [] = { 0, 0, 0, 0, 0, 1, 2, 5};
const unsigned char pc_scores_b [] = { 1, 2, 3, 4, 5, 0, 0, 0};

// Sounds for items
const unsigned char sfx_item [] = {
	SFX_ITEM_0, SFX_ITEM_1, SFX_ITEM_2, SFX_ITEM_3,
	SFX_ITEM_4, SFX_ITEM_5, SFX_ITEM_6, SFX_ITEM_7
};

#ifdef CNROM
const unsigned char bg_anim [] = { 0, 1, 0, 2 };
#endif

const unsigned char score_offsets [] = { 0, 5 };
