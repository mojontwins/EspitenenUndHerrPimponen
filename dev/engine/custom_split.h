// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// Custom split. Waits for sprite 0 hit, then set the
// scroll registers. Note that there's an offset of 24 pixels.

void custom_split (void) {

	__asm__ ("lda %v", scroll_y);
	
	__asm__ ("and #$f8");
	__asm__ ("asl a");
	__asm__ ("asl a");
	__asm__ ("sta %v", rdy);

	__asm__ ("lda #0");
	__asm__ ("ldx %v", scroll_y);
	__asm__ ("ldy %v", rdy);


SPLITW0:
	__asm__ ("bit PPU_STATUS");
	__asm__ ("bvs %g", SPLITW0);

SPLITW1:
	__asm__ ("bit PPU_STATUS");
	__asm__ ("bvc %g", SPLITW1);


// We need 4 writes:
// $2006 = NT << 2 = 0;
// $2005 = Y
// $2005 = X = 0;
// $2006 = (Y & $F8) << 2 | (x >> 3) = (Y & $F8) << 2
// All of which is precalculated.

	__asm__ ("sta $2006");
	__asm__ ("stx $2005");
	__asm__ ("sta $2005");
	__asm__ ("sty $2006"); 

}
