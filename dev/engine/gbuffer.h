// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// gbuffer.h - Circular game buffer

// This buffer contains a representation of an upacked screen
// (which is more or less what's shown on screen at once)

// BEWARE!
// I've made fixed room for this buffer in RAM modifying nes.cfg

// Buffer is @300

// Need this on ZP:
// unsigned char gbuffer_y;

void cm_two_points (void) {
	at1 = behs [SCREEN_BUFFER [cx1 | ((cy1 & 15) << 4)]];
	at2 = behs [SCREEN_BUFFER [cx2 | ((cy2 & 15) << 4)]];
}
