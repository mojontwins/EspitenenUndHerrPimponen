// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// BSS variables

// 50 or 60 depending on TV system

unsigned char ntsc, ticks, halfticks;

// Flow

unsigned char level, game_time, ticker, tick, display;
unsigned char flash, free_frame, fskipct;
unsigned char scroll_speed, goal_time, sadv;
unsigned char pad0, pad1, prevpad, pad_once, pad_doce, half_life, frame_counter, hl_proc;
unsigned char game_mode, collide_players, respawn_players;
unsigned int nt;
unsigned char do_split;
unsigned char pselected;

// Update list for VBlank updates

unsigned char update_list [UPDATE_LIST_SIZE * 3 + 1];

// Fade in / fade out

signed char fader;

// Camera control

unsigned char is_scrolling;
unsigned char autoscroll;
signed int scroll_to;

// Bullets (cont)

signed int bx [BULLETS_MAX];
unsigned int by [BULLETS_MAX];
signed char bvx [BULLETS_MAX], bvy [BULLETS_MAX];
unsigned char ba [BULLETS_MAX];

// Player (cont)

unsigned char pflickers;
unsigned char p_pressingA, p_pressingB;
unsigned char pdpadaccelerates, pndpaddeccelerates;
unsigned char pcontinues;

// Enems (cont)

unsigned int eny1 [ENEMS_MAX];
unsigned char enx1 [ENEMS_MAX], enx2 [ENEMS_MAX], enct [ENEMS_MAX];
signed char enmx [ENEMS_MAX], enmy [ENEMS_MAX];
unsigned char ena [ENEMS_MAX];
unsigned char ent [ENEMS_MAX];
unsigned char enl [ENEMS_MAX];
unsigned char enhitbg;
unsigned char entypekillable [8];
unsigned char enstart;

// BCD score

unsigned char bcd_score [10];
unsigned char bcd_high_score [5];

// Points sprites

unsigned char points_x [POINTS_MAX], points_y [POINTS_MAX], points_id [POINTS_MAX], points_fc [POINTS_MAX];
unsigned char points_slots_index;

// Puas

#ifdef PLAYER_PUAS
unsigned char puas_x [PUAS_MAX];
unsigned int puas_y [PUAS_MAX];
signed char puas_mx [PUAS_MAX];
unsigned char puas_st [PUAS_MAX];
unsigned char puas_who [PUAS_MAX];
#endif

// Slippery aid

#ifdef PLAYER_SLIPPERY
signed char _PLAYER_AX, _PLAYER_RX;
unsigned char pslips;
#endif

unsigned char pal_cycle_do;
unsigned char pal_cycle [3];

// Signal every 10000 points.

unsigned char signal10000;

// Continue?

unsigned char signal_continue;

// Two players!
unsigned char round;
unsigned char level_selected;
unsigned char winner;
unsigned char pscore [2];

// 8 bit
signed char apvx [2], apvy [2];
unsigned char aprx [2];
unsigned char apfacing [2];
unsigned char apkill [2];
signed char aptgmx [2];
unsigned char aplives [2], aplife [2];
unsigned char apst [2], apct [2];
unsigned char apsinking [2];
unsigned char ap_pressingA [2], ap_pressingB [2];
unsigned char apj [2], apctj [2], appossee [2], apgotten [2];
unsigned char apon [2];
unsigned char apcharacter [2];

// 16 bit
unsigned int apy [2], apcy [2];
signed int apx [2], apcx [2];
signed int apry [2];
