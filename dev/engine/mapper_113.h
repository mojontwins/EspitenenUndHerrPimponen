// MT MK2 NES v0.7
// Copyleft 2016 by The Mojon Twins

// Communication pool utilites.

#define COMM_POOL						((unsigned char*)0x0300)	// Mapper 113 stuff.

#define COMM_REG						(COMM_POOL[0x0])
#define COMM_CHECKSUM 					(COMM_POOL[0xf])

void m113_comm_pool_checksum_calculate (void) {
	rda = 0; for (gpit = 1; gpit < 15; gpit ++) rda += COMM_POOL [gpit];
}

void m113_safe_change_chr_bank (unsigned char chr) {
	rda = COMM_REG & 0xb8; // MCPPPCCC -> 10111000
	if (chr & 8) { rda |= 0x40; chr &= 0x7; }
	rda |= chr;
	COMM_REG = rda; 
	__asm__ ("jmp _change_reg");
}

// You can comment out these from "head" ROM if space is tight:

unsigned char m113_comm_pool_checksum_check (void) {
	m113_comm_pool_checksum_calculate ();	// Written to rda
	return (COMM_CHECKSUM == rda);
}

void m113_handle_reset (void) {
	if (m113_comm_pool_checksum_check ()) {
		COMM_CHECKSUM = 0xff; 				// Bad
		return;
	}
	COMM_GAME_SELECT = 0x00;
	COMM_REG = 0x80; // MCPPPCCC
	__asm__ ("jmp _change_rom");
}

// You can comment out these from "child" ROMs if space is tight:

void m113_comm_pool_init (void) {
	for (gpit = 1; gpit < 15; gpit ++) COMM_POOL [gpit] = gpit;
}

void m113_comm_pool_checksum_write (void) {
	m113_comm_pool_checksum_calculate ();	// Written to rda
	COMM_CHECKSUM = rda;
}

void m113_rom_pair_jump (unsigned char prg, unsigned char chr) {
	rda = 0x00; if (chr & 8) { rda |= 0x40; chr &= 0x7; }
	rda |= (prg << 3); 
	rda |= chr;
	COMM_REG = rda; 
	__asm__ ("jmp _change_rom");
}
