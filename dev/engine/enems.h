// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// Enemies!

void player_gotten (void) {
	pgotten = 1;
	ptgmx = (envx << FIX_BITS);
	pry = _eny - 16; py = pry << FIX_BITS;
}

void enems_init (void) {
	enit = ENEMS_MAX; while (enit --) {
		ena [enit] = 0; enslots [enit] = enit;
	}
	ensloti = ENEMS_MAX;
	enstart = 0; 
}

#include "engine/enem_mods/enems_spawn_assembly.h"

void enems_destroy (void) {
	// It's just a matter of freeing its slot. That is, put the
	// slots at the end of the stack of free slots.

	ena [enit] = 0;
	enslots [ensloti] = enit;
	ensloti ++;
}

void enems_kill (void) {
	// Turns the current baddie into an explosion (!)

	ent [enit] = 16;
	_enct = 16;
	rda = 1;
}

 #include "engine/enem_mods/enems_do_assembly.h"
