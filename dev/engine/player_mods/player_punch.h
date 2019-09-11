// Pantanow Engine rev5 201906
// Copyleft 2017, 2019 by The Mojon Twins

	if (ppunching) {
		ppunching --;
		ppunchx = pfacing ? prx - 12 : prx + 12;
		ppunchy = pry;
	} else ppunchy = 0xffff;
