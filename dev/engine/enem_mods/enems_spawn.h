// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

void enems_spawn (void) {
	rdy = strip_pointer [0];
	*((unsigned char *) 0xf0) = rdy; // An optimization breaks my code.
	if (rdy == map_tilepos && rdy) {
		rdx = strip_pointer [1]; rdc = strip_pointer [2];

		// rdx == XXXXTTTT rdc == BBBBAAAA

		// Create enemies upon type etc.
		// Scroll lock goes here, when needed, etc.
		// You better make a mix with stuff from here and
		// stuff from Pantanow. Pretty custom, if you ask.

		// Find a free slot. If sloti == 0, no free slots & do nothing.
		if (ensloti) {
			ensloti --;
			enit = enslots [ensloti];

			ena [enit] = 1;			// Enemy active
			
			// Now fill everything else. Position, state, counters...

			ent [enit] = rdx & 0x0f; 		// TTTT
			enx [enit] = 4 + (rdx & 0xf0);	// XXXX
			eny [enit] = rdy << 4;			// YYYYYYYY
			eny1 [enit] = eny [enit];
			enl [enit] = 0;

			rda = rdc & 0x0f;
			rdb = rdc & 0xf0;
			
			switch (ent [enit]) {
				#ifndef ENEM_DISABLE_BULLETS
					case 1:
						// Fixed
						ENFREQ [enit] = rda; // Means shoot freq.
						break;
				#endif
				case 2:
					// Back & Forth, stop @ obstacles / holes
				case 3:
					// Fallers, like 2 but fall.
					if (rda == 0) {
						// half pixel
						enx1 [enit] = 0; rda = 1;
					} else enx1 [enit] = 1;
					enmx [enit] = rdb & 0x10 ? -rda : rda;
					enmy [enit] = 0;
					break;
				case 4: 
					// Chasers
					enmx [enit] = enmy [enit] = 0;
					break;
#ifndef ENEM_DISABLE_THROWABLES
				case 5:
					// Arrows
					// B == FOxx, A = time
					ENMAXCT [enit] = 8 + (rda << 3);
					ENSTATE [enit] = 3;
					enmx [enit] = 0;
					enct [enit] = 0;
					enmy [enit] = 0;
					break;
#endif
				case 6:
					// flipflop platforms
					ENSTATE [enit] = 0; // off
					enct [enit] = enx [enit] & 0x7f;
					break;
				case 7:
					// Platforms
					enx1 [enit] = rda << 4;
					enx2 [enit] = rdb;
					if (enx1 [enit] < rdb) {
						enmx [enit] = 1;
					} else {
						enmx [enit] = -1;
						// Swap limits so x1 < x2 always
						rdd = enx1 [enit];
						enx1 [enit] = enx2 [enit];
						enx2 [enit] = rdd;
						enx [enit] = enx2 [enit];
					}
					break;
				case 9: 
					// Green boss
					ENFREQ [enit] = rda;
					enmx [enit] = enmy [enit] = 1;
					enl [enit] = 4;
					break;
			}
		}

		// And advance pointer. If an enemy was skipped... bad luck.
		if (rdy) strip_pointer += ENEM_SIZE;
	}
}
