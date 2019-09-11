// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// Main definitions

	#define SCREEN_BUFFER_V				0x300
	#define ATTRIB_BUFFER_V				0x400
	#define SCROLL_BUFFER_V				0x440

	#define SCREEN_BUFFER 				((unsigned char*)SCREEN_BUFFER_V)
	#define SCROLL_BUFFER				((unsigned char*)SCROLL_BUFFER_V)
	#define ATTRIB_BUFFER 				((unsigned char*)ATTRIB_BUFFER_V)

	#define DBG_POOL					((unsigned char*)0xf0)	

	#define SPLIT_SIZE 					24
	#define SCROLL_Y 					(-16)
	#define UPDATE_LIST_SIZE			32
	#define MSB(x)						((x)>>8)
	#define LSB(x)						((x)&0xff)
	#define SPRITE_ORIGIN				0
	#define TILE_ORIGIN 				0
	#define SPRITE_ADJUST				SPRITE_ORIGIN-1-SCROLL_Y
	#define TOP_ADJUST					0
	#define FIX_BITS					4
	#define FADE_DELAY 					1

	#define SGNI(n)						((n)<0?-1:1)
	#define SGNC(a,b,c)					((a)<(b)?-(c):(c))
	#define SATURATE_Z(n)				((n)<0?0:(n))
	#define SATURATE(v,max)				((v)>=0?((v)>(max)?(max):(v)):((v)<-(max)?-(max):(v)))
	#define ABS(n)						((n)<0?-(n):(n))
	#define DELTA(a,b)					((a)<(b)?(b)-(a):(a)-(b))
	//#define CL(x1,y1,x2,y2)			((x1)+4>=(x2)&&(x1)<=(x2)+12&&(y1)+12>=(y2)&&(y1)<=(y2)+12)
	#define CLE(x1,y1,x2,y2)			((x1)+7>=(x2)&&(x1)<=(x2)+7&&(y1)+14>=(y2)&&(y1)<=(y2)+14)
	#define CLB(x1,y1,x2,y2)			((x1)+7>=(x2)&&(x1)<=(x2)+7&&(y1)+8>=(y2)&&(y1)<=(y2)+14)
	#define SUBSTR_SAT(a,b)				((a)>(b)?(a)-(b):0)
	#define DISABLE_SFX_NEXT_FRAME		*((unsigned char*)0x01bf)=1;
	#define ADD_SIGN2(a,b,v)			((a)==(b))?(0):(((a)>(b))?(v):(-(v)))
	#define ADD_SIGN(a,v) 				((a)?(((a)>0)?(v):(-(v))):(0))

// Change this if you change where digits are stored in the pattern table
	#define DIGIT(n) 					(16+(n))

// Game definitions
	#define MAP_H						128

// General game stuff
	#define AUTOSCROLL					// Uncoment to make camera follow the player

// Goame modes
	#define GM_SINGLE					0
	#define GM_COOP						1
	#define GM_VS						2

// Player
	#define PLAYER_LIVES_INI			5
	#define PLAYER_LIFE_INI				3

	//#define PLAYER_HAS_PUNCH
	#define PLAYER_PUAS
	#define PUAS_MAX					2
	#define PUAS_VX						8
	#define PUAS_PATTERN				17

	#define PLAYER_EVIL_TILE
	#define PLAYER_EVIL_TILE_BIT		1

	#define PLAYER_TILE_GET				// Define this and implement logic!
	#define PLAYER_TILE_GET_BIT			2

	#define PLAYER_SLIPPERY
	#define PLAYER_SLIPPERY_BIT			16

	#define PLAYER_SINKS
	#define SINK_BIT					32
	#define PLAYER_VY_SINKING			1 // 0xff
	#define PLAYER_VY_EXIT_SINKING		8
// Gravity
	#define PLAYER_G					4
	#define PLAYER_G_JUMPING			1
	#define PLAYER_VY_FALLING_MAX		80

// Jump
	#define PLAYER_VY_JUMP_INITIAL		56
	#define PLAYER_AY_JUMP				8
	#define PLAYER_VY_JUMP_A_STEPS		16
	#define PLAYER_VY_JUMP_BUTT			64
	#define PLAYER_VY_JUMP_RELEASE		24

// General behaviour bits
	#define OBSTACLE_BIT				8
	#define PLATFORM_BIT				4
	#define FLOOR_BITS					(OBSTACLE_BIT|PLATFORM_BIT)
	#define POSEE_BITS					(FLOOR_BITS|SINK_BIT)
	#define ENEMS_COLLIDER_BIT			16	// Extra collider for enems
	#define ENEMS_OBSTACLE_BITS			(OBSTACLE_BIT|ENEMS_COLLIDER_BIT)
	#define ENEMS_FLOOR_BITS			(FLOOR_BITS|ENEMS_COLLIDER_BIT|SINK_BIT)

// Printer stuff
	#define PRINT_PSEUDOASCII				// patterns 0-63 == ASCII 32-95
	//#define PRINT_CUSTOM					// Custom 0xff-terminated strings (mkts)

// State
	#define EST_DEAD 					2
	#define EST_FLICKER 				1
	#define EST_NORMAL 					0

// Horizontal
	#define PLAYER_AX					4
	#define PLAYER_RX					3
	#define PLAYER_AX_SLIPPERY			1
	#define PLAYER_RX_SLIPPERY			1
	#define PLAYER_VX_MAX				40
	#define PLAYER_VX_REBOUND			64
	#define PLAYER_VX_MIN 				(PLAYER_AX << 2)

// Enems
	#define ENEMS_MAX 					8
	#define EN_FACING					4	// Add this to turn left
	#define ENEM_SIZE 					3	// Size of each entry in ROM
	#define ENEM_G						7
	#define ENEM_A						2
	#define ENEM_MAX_V					16
	#define ENEM_MAX_VFALL				96
	#define ENEM_DISABLE_BULLETS			// If you won't be using them...
	#define ENEM_DISABLE_THROWABLES			// No drops / arrows & etc

	#define ENEM_SHOOT_TEST				1	// the bigger the more they shoot

// Bullets
	#define BULLETS_MAX					8
	#define BULLETS_V					32
	#define BULLETS_PATTERN				2
	#define BULLETS_PALETTE 			1

// Cells
	#define CELL_FACING_LEFT			8
	#define CELL_FACING_RIGHT 			0

	#define CELL_IDLE 					0
	#define CELL_WALK_BASE				1
	#define CELL_AIRBORNE				5
	#define CELL_DEAD					7
	//#define CELL_PUNCHING				7

// Sprites for points
	#define POINTS_ENABLE
	#define POINTS_MAX					4

// Sound FX
	#define SFX_START					0

	#define SFX_JUMP					1
	#define SFX_PUAS					9
	#define SFX_PLAYER_HIT				3
	
	#define SFX_ENEMY_HIT				6

	#define SFX_ITEM_0					18
	#define SFX_ITEM_1					5
	#define SFX_ITEM_2					8
	#define SFX_ITEM_3					17
	#define SFX_ITEM_4					16
	#define SFX_ITEM_5					19
	#define SFX_ITEM_6					20
	#define SFX_ITEM_7					2
	
	#define SFX_STAR					4

	#define SFX_FANFARE					21

// Sound channels (priorities)
	#define SC_MAIN						0
	#define SC_PLAYER					1
	#define SC_ROOM 					2

// Music
	#define MUSIC_INGAME				0

// Handy redefinitions for clearer code (at least methinks so!)
#ifndef PLAYER_SLIPPERY
	#define _PLAYER_AX 					PLAYER_AX
	#define _PLAYER_RX					PLAYER_RX
#endif
	