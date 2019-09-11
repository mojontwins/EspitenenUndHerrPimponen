// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// Zero page variables

//const unsigned char * const *spr_player;
const unsigned char * const *spr_enems;

// Level (fast) pointers

const unsigned char *c_ts_tmaps_0, *c_ts_tmaps_1, *c_ts_tmaps_2, *c_ts_tmaps_3;
const unsigned char *c_ts_pals_0, *c_ts_pals_1, *c_ts_pals_2, *c_ts_pals_3;
unsigned char c_level_killable_enems;
const unsigned char *c_spr_base;

// General stuff

const unsigned char *map_ptr;
unsigned char *gp_gen;
unsigned char map_tilepos;
const unsigned char *behs;
unsigned char *ul;
unsigned int gp_addr;

unsigned char oam_index;

unsigned char gpit, gpjt;
unsigned char rda, rdb, rdc, rdd, rdt;
signed char rds;
signed int rds16;
unsigned int rdu16;
unsigned char rdx; //, rdy;
signed int rdy;
//signed char rdmx, rdmy;
signed int gpiit, gpijt;
unsigned int uin;

// RLE Scroller
signed int cam_pos;
unsigned char cam_pos_lsb;
unsigned char cam_pos_bit_4;
unsigned char cam_pos_bit_3;
unsigned char scroll_state;

unsigned char scroll_y;			// scroll position (nametables)
unsigned char o_scroll_y;			// scroll position (nametables)
unsigned char wtp, wtpr;		// Current line to write (nametables)

unsigned char *rle_buffer_ptr;	// Pointing to the SCREEN buffer
unsigned char rle_buffer_i;
unsigned char rle_ctr, rle_value;

unsigned char attr_buffer_i; // Pointing to the ATTR buffer

// Scroller pointers

//unsigned char *gpp;
unsigned char gppi;
unsigned char gpri;
unsigned char *gpq;

// Buffer

unsigned char gbuffer_y;

// Player

unsigned char pad;

unsigned int py;
signed int px;
signed char pvx, pvy;
unsigned char prx;
signed int pry;
unsigned char psprid, pfacing, pkill, ppunching;
signed char ptgmx;
unsigned char plives, plife;
unsigned char pst, pct;
signed int gpint;
unsigned char pj, pctj, ppossee, pgotten;
unsigned char pindex;
unsigned char score_offset;
unsigned char pcharacter;

#ifdef PLAYER_SINKS
	unsigned char psinking;
#endif

#ifdef PLAYER_HAS_PUNCH
	unsigned char ppunchx; 
	unsigned int ppunchy;
#endif

// Collision

unsigned char cx1, cy1, cx2, cy2, at1, at2;
unsigned char caux;
unsigned char _d1, _d2;

// Enems strip

const unsigned char *strip_pointer;

// Enems

unsigned char enit, enjt;

unsigned char enx [ENEMS_MAX];
unsigned int eny [ENEMS_MAX];
unsigned char encx;
signed char envx, envy;

unsigned char _enct, _enx, _enx1, _enx2;
signed char _enmx, _enmy;
unsigned int _eny;

#define ENMAXCT enx1
#define ENSTATE enx2
#define ENFREQ enx1

#define _ENMAXCT _enx1
#define _ENSTATE _enx2
#define _ENFREQ _enx1

// Enems slots (stack)

unsigned char ensloti;
unsigned char enslots [ENEMS_MAX];

// Bullets

unsigned char bit;
unsigned int bpy;

// Bullets slots (stack)

unsigned char bsloti;
unsigned char bslots [BULLETS_MAX];

// Bullets aux

signed int _bx;
unsigned int _by;

#ifdef DEBUG
unsigned char debug_on;
#endif

// Puas

unsigned char _puas_x;
signed char _puas_mx;
unsigned int _puas_y;
