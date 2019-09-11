// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

// General assorted stuff

void get_pad_once (void) {
	// Thanks for this, Nicole & nesdev!
	// https://forums.nesdev.com/viewtopic.php?p=179315#p179315
	pad_once = pad0; 
	pad0 = pad_poll (0);
	pad_once = (pad_once ^ pad0) & pad0;

	pad_doce = pad1;
	pad1 = pad_poll (1);
	pad_doce = (pad_doce ^ pad1) & pad1;
}
