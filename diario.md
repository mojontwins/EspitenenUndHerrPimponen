20190611
========

En super secreto voy a ver cómo de viable es hacer dos jugadores en espinete. Primero voy a medir cuántos frames ocupa la rutina de mover a espinete, y luego cuánto tiempo de frame hay libre. En general. Luego tengo que optimizar a saco algunas rutinas, las básicas para empezar.

Es muy variable. Hay demasiadas cosas que pueden estar o no. Quizá es muy arriesgado. Está claro que por ejemplo, las puntuaciones que salen volando al tomar los objetos podría pasarlas a ensamble, y las rutinas del printer también (quizá pillarlas directamente de vigirdo o mk1).

La rutina de movimiento de Espitene está siempre por debajo de 32 rasters, lo cual podría ser suficiente. Ese tiempo se duplicaría, y además tendría que hacer la multiplexión: copiar de arrays a variables en ZP y redirigir el control del pad ocupará algunos rasters.

Creo que es viable pero antes de ponerme tendría que optimizarlo todo, ya que por ejemplo estaré duplicando las colisiones con los malos.

Bien.

Ahora la rutina de scroll. En C sube de los 32 rasters en algunos pasos, porque tiene muchas secciones de escribir en puntero, incrementar puntero.

Hay que tener en cuenta que todo esto se escribe en SCROLL_BUFFER y ATTRIB BUFFER, que están en 0x310 y 0x330, respectivamente, por lo que NUNCA HAY SALTO DE PÁGINA. Esto puede y debe ser tomado en consideración ¿no?

Vamos al líow:

```c
	// Unrolled, 8 times	
	rda = *rle_buffer_ptr ++;
	*gpp ++ = c_ts_tmaps_2 [rda];
	*gpp ++ = c_ts_tmaps_3 [rda];
	rda = *rle_buffer_ptr ++;
	*gpp ++ = c_ts_tmaps_2 [rda];
	*gpp ++ = c_ts_tmaps_3 [rda];
	rda = *rle_buffer_ptr ++;
	*gpp ++ = c_ts_tmaps_2 [rda];
	*gpp ++ = c_ts_tmaps_3 [rda];
	rda = *rle_buffer_ptr ++;
	*gpp ++ = c_ts_tmaps_2 [rda];
	*gpp ++ = c_ts_tmaps_3 [rda];
	rda = *rle_buffer_ptr ++;
	*gpp ++ = c_ts_tmaps_2 [rda];
	*gpp ++ = c_ts_tmaps_3 [rda];
	rda = *rle_buffer_ptr ++;
	*gpp ++ = c_ts_tmaps_2 [rda];
	*gpp ++ = c_ts_tmaps_3 [rda];
	rda = *rle_buffer_ptr ++;
	*gpp ++ = c_ts_tmaps_2 [rda];
	*gpp ++ = c_ts_tmaps_3 [rda];
	rda = *rle_buffer_ptr ++;
	*gpp ++ = c_ts_tmaps_2 [rda];
	*gpp ++ = c_ts_tmaps_3 [rda];
```

Aquí tenemos gpp, que apunta a `SCROLL_BUFFER`. Vamos a pasar de eso, en su lugar vamos a usar un índice, gppi. Muevo `rle_buffer` a ZP también. Vamos, un puto rewrite.

`rle_buffer_ptr` también caerá a favor de `rle_buffer_idx`.

Wait, esto está optimizado ya. `rle_buffer` ya no se usa; se apunta con `rle_buffer_ptr` directamente a la linea correcta del buffer de pantalla SCREEN_BUFFER.

Bueno, para empezar a optimizar, voy a quitar `SCREEN_BUFFER` de un array definida en C y la voy a colocar a pelaco en el mapa de memoria. Necesito toda una páquina, así que desplazaré todo lo demás hacia arriba y la colocaré en 0x300. De ese modo, `SCROLL_BUFFER` se mueve a 0x410, y `ATTRIB_BUFFER` a 0x430. Esto elimina el "mode 13 friendly", pero me chupa un pie por ahora.

Ahora lo he roto XD

El mapa está bien relleno, pero los tiles no se pintan. O no scrollea de donde debe. O yo qué sé.

Veamos: 
- El buffer circular se llena divinamente. 
- El buffer de atributos y el de patrones se llenan divinamente.

Esto me suena a que neslib los tenga hardcodeados, obviously, y se me chisporroteó cambiarlos :-D

DIGO. Qué tontete soy :-D

Ahora, con todo en su sitio, puedo empezar a pasar esto a asm obviando mucho, mucho código.

Primeramente, voy a acceder como arrays. Prototiparé primero en C y luego paso a ASM. Solo el cambio en C debería acelerar bastante la rutina, viendo cómo genera el código cc65. Y luego mis optimizaciones serán modificar ese código sabiendo que jamás voy a saltar de página.

He hecho el cambio en la parte que escribe los patrones y funciona. Ahora voy a optimizar de la misma forma la parte que escribe los atributos.

Cambio gpr (puntero) por gpri (índice).

Ahora mismo tal que así Estamos ahorrando bastante: se ha quedado por debajo, aunque cerca, de los 32 rasters. 

Ahora mismo todo el cálculo de los atributos lo hace de forma explícita. Hay muchos desplazamientos que pueden ser la muerte con pelos.

Ahora mismo tengo los patrones desenrollados en 4 arrays; lo suyo sería que tuviese los atributos precalculados en cuatro arrays también. No sé si el mkts hacía esto ya, en todo caso es fácil de apañárselo. De esa forma me ahorraría toneladas de desplazamientos!

Misteriosamente no parece haber mejorado mucho en el peor caso - ¿quizá el peor caso es el de los patrones? Sea como fuere, pasar a ASM sí que va a mejorar porque en este tipo de accesos repetitivos desenrollados a posiciones consecutivas el código a mano suele ser mucho mejor.

Patrones
--------

Ahora mismo cada uno de los desenrollados (copia medio metatile) de la parte de patrones genera todo este asm:

```s
	;
	; rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
	;
	L2B6B:	lda     _rle_buffer_ptr
		ldx     _rle_buffer_ptr+1
		ldy     _rle_buffer_i
		sta     ptr1
		stx     ptr1+1
		lda     (ptr1),y
		sta     _rda
		inc     _rle_buffer_i
	;
	; SCROLL_BUFFER [gppi] = c_ts_tmaps_2 [rda]; ++ gppi;
	;
		ldx     #$04
		lda     #$10
		clc
		adc     _gppi
		bcc     L2B75
		inx
	L2B75:	jsr     pushax
		lda     _c_ts_tmaps_2
		ldx     _c_ts_tmaps_2+1
		ldy     _rda
		sta     ptr1
		stx     ptr1+1
		lda     (ptr1),y
		ldy     #$00
		jsr     staspidx
		inc     _gppi
	;
	; SCROLL_BUFFER [gppi] = c_ts_tmaps_3 [rda]; ++ gppi;
	;
		ldx     #$04
		lda     #$10
		clc
		adc     _gppi
		bcc     L2B7C
		inx
	L2B7C:	jsr     pushax
		lda     _c_ts_tmaps_3
		ldx     _c_ts_tmaps_3+1
		ldy     _rda
		sta     ptr1
		stx     ptr1+1
		lda     (ptr1),y
		ldy     #$00
		jsr     staspidx
		inc     _gppi
```

Creo que es fácilmente reducible sabiendo las características de nuestras estructuras de datos. Vamos al tema.

Lo he dejado en esto (con pequeños prólogo y epílogo):

```c
// X = gppi

__asm__ ("ldy %v", rle_buffer_i);
__asm__ ("lda (%v), y", rle_buffer_ptr);
__asm__ ("tay");
__asm__ ("inc %v", rle_buffer_i);

// Y = rle_buffer_ptr [rle_buffer_i]

__asm__ ("lda (%v), y", c_ts_tmaps_2);
__asm__ ("sta %w, x", SCROLL_BUFFER);
__asm__ ("inx");

__asm__ ("lda (%v), y", c_ts_tmaps_3);
__asm__ ("sta %w, x", SCROLL_BUFFER);
__asm__ ("inx");
```

Voy a ver si funfuña (solo está para una mitad del metatile). ¡¡Funfuña!! Voy a por el otro bloque.

Veamos cuanto ocupan los bloques de patrones... alrededor de 4 rasters. Veamos los otros bloques.

Estados 1, 2, 5, 6 pintan patrones.
El estado 3 pinta atributos. Este es el que se tira LO MAS GRANDE. Ahora mismo cada vuelta desenrollada genera esto:

```s
	; rda = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
	;
		lda     _rle_buffer_ptr
		ldx     _rle_buffer_ptr+1
		ldy     _rle_buffer_i
		sta     ptr1
		stx     ptr1+1
		lda     (ptr1),y
		sta     _rda
		inc     _rle_buffer_i
	;
	; rdb = rle_buffer_ptr [rle_buffer_i]; ++ rle_buffer_i;
	;
		lda     _rle_buffer_ptr
		ldx     _rle_buffer_ptr+1
		ldy     _rle_buffer_i
		sta     ptr1
		stx     ptr1+1
		lda     (ptr1),y
		sta     _rdb
		inc     _rle_buffer_i
	;
	; ATTRIB_BUFFER [gpri] = (ATTRIB_BUFFER [gpri] & 0x0f) | c_ts_pals_2 [rda] | c_ts_pals_3 [rdb];
	;
		ldx     #$04
		lda     #$30
		clc
		adc     _gpri
		bcc     L2C37
		inx
	L2C37:	jsr     pushax
		ldx     #$04
		lda     #$30
		ldy     _gpri
		sta     ptr1
		stx     ptr1+1
		lda     (ptr1),y
		and     #$0F
		jsr     pusha0
		lda     _c_ts_pals_2
		ldx     _c_ts_pals_2+1
		ldy     _rda
		sta     ptr1
		stx     ptr1+1
		lda     (ptr1),y
		jsr     tosora0
		jsr     pushax
		lda     _c_ts_pals_3
		ldx     _c_ts_pals_3+1
		ldy     _rdb
		sta     ptr1
		stx     ptr1+1
		lda     (ptr1),y
		jsr     tosora0
		ldy     #$00
		jsr     staspidx
	;
	; ++ gpri;
	;
		inc     _gpri
	;

```

Lo cual no parece demasiado, al menos NO MUCHO MÁS que lo que generaba la otra parte; sin embargo dura MUCHO MÁS. Gran misterio.

Voy a ver qué tal se da reescribirlo en ASM...

Mis vueltas son (con pequeña inicialización):

```c
	__asm__ ("ldy %v", rle_buffer_i);

	__asm__ ("lda (%v), y", rle_buffer_ptr);
	__asm__ ("sta %v", rda);
	__asm__ ("iny");
	__asm__ ("lda (%v), y", rle_buffer_ptr);
	__asm__ ("sta %v", rdb);
	__asm__ ("iny");

	__asm__ ("sty %v", rle_buffer_i);

	__asm__ ("lda %v, x", ATTRIB_BUFFER);
	__asm__ ("and #0x0f");

	__asm__ ("ldy %v", rda);
	__asm__ ("ora (%v), y", c_ts_pals_2);

	__asm__ ("ldy %v", rdb);
	__asm__ ("ora (%v), y", c_ts_pals_3);

	__asm__ ("sta %v, x", ATTRIB_BUFFER);

	__asm__ ("inx");
```

Lo cual debería ser *bastante* más efectivo... Si funciona. ¡Funciona! Cambio la otra parte y mido tiempos :-)

¡JOJO! Lo he dejado en 4-5 rasters! Voy a limpiar un poco el código y mido el tiempo de toda la rutina y veo como oscila. Es que con lo que me he ahorrado y poco más tendré para meter al segundo jugador :-D

Hm. Sigue habiendo un paso que me jode todo, que se pone en 30 y pico ¿qué se me está escapando?

Voy a ver si es el caso 4... No, no es el caso 4. Algo se me escapa. El caso "0" no está en el select.

Digo. El caso que más ocupa es el que hace esto:

```c
	if (rda != cam_pos_bit_4) {
		cam_pos_bit_4 = rda;
		cam_pos_bit_3 = cam_pos & 0x08;

		// Fill buffer
		map_tilepos --;
		gbuffer_y = gbuffer_y - 16;
		rle_buffer_ptr = gpq = SCREEN_BUFFER + gbuffer_y;
		gpit = 16; while (gpit --) *gpq ++ = scroll_get_byte ();		

		// Init indexes

		//gpp = SCROLL_BUFFER;		
		//gpr = ATTRIB_BUFFER;
		gpri = gppi = rle_buffer_i = 0;

		// Relocate writers
		scroll_writers_realloc ();

		scroll_state = 1;
		return;
	}
```

Y es por el puto bucle que hay ahí: la decompresión. Hay un bucle, una llamada a `scroll_get_byte` ... Me parece que por mucho que apriete esto no lo dejo tan pincelín como el otro.

A lo mejor es el momento de reestructurar los estados y hacer la decompresión en dos trozos. Ahora estoy haciendo todo en 7 estados marcando el estado 7 como free frame - y eso no se usa para nada.

OK, restructuramos. Ocuparemos los dos primeros estados con la descompresión, en dos pasos, y los seis restantes con el resto.

Vale. Lo he dejado (aún en C) en menos de 16 rasters como máximo por vuelta. Voy a pasar a ASM esta parte lo mejor que pueda, por pasos. Primero la función `scroll_get_byte`. Lo primero que he hecho es una mini mejora para quitar el `>> 5` que había. Ahora voy a pasar a asm...

OK - mi código es bastante más sencillo. Veamos si funciona. No funciona. Sí funciona. Fallo mensísimo.

Como voy a llamar a la función desde ASM me follo el unsigned char y el return.

¡Misión cumplida! MENOS DE 8 RASTERS!

20190612
========

He pasado a ensamble otras miserias como las puas o los puntos y creo que ya me encuentro preparado para liarla parda y meter dos jugadores. Hay una serie de variables que tendré que ir sacando y metiendo de arrays, las variables que controlan todo el estado del jugador:

```c
	unsigned int py;
	signed int px;
	
	signed char pvx, pvy;
	
	unsigned char prx;
	signed int pry;

	unsigned char pfacing;	
	unsigned char pkill;

	signed char ptgmx;
	unsigned char plives, plife;

	unsigned char pst, pct;
	unsigned char psinking;

	unsigned char p_pressingA, p_pressingB;

	unsigned char pj, pctj, ppossee, pgotten;
```

Por ahora voy a ir creando los arrays. BIEN. Caben en memoria. Ahora necesito una función players_move que:

1. Compruebe si el player N está activo.
2. Copie todas sus variables
3. Llame a player_move
4. Restaure todas sus variables.

Hecho. Voy a probarlo activando solo un jugador. Obviamente si no hago más cambios toda la colisión con los objetos móviles se referirá al último jugador ejecutado.

Hablando de todo un poco, quizá podría ahorrar alternando el orden de ejecución: 0, 1 y luego 1, 0. Así las comprobaciones que digo se harían cada 2 frames con cada jugador, alternativamente. Pero lo veo arriesgado.

Bueno, voy probando.

~~

El movimiento funciona y no se va de frame ni hostias en vinagre ni nada, y eso que aún tengo marco para optimizar más :-)

Ahora lo chungo son las interacciones... Y con esto me refiero a dos cosas muy diferentes: las colisiones con los otros actores (enemigos y plataformas) y luego toda la lógica subyacente de jugar: modos 1P, 2PC, 2PVS, colisión entre los espinetes, etc.

Y luego hacer un segundo personaje para el que ni siquiera tengo claro que haya paleta libre. 

A ver, lo más trivial es meter en un bucle esto, asignando las variables en juego.

Obviously la he liado porque en cuanto meto estas comprobaciones es como si los dos espinetes volviesen a ser uno.

A ver qué pata he metido... Es obvio que no está actualizando los arrays que debe.

~~

Veamos. Ya tengo los dos jugadores, y sus interacciones con los malos, y que empiecen el scroll. El final de las fases debería estar básicamente controlados. Me queda:

1.- Menú: 1P, 2PC, 2Pvs
2.- En modo 2Pvs, mostrar dos marcadores? Esto va a estar chungo, porque no puedo meter tantos sprites :-/ Tengo que pensar cómo hacerlo. ¿Otro tipo de puntuación?
3.- En modo 2Pvs y probablemente en 2PC, los jugadores deberían chocar y aplicarse la ley de conservación del movimiento.
4.- En modos 2Pvs y 2PC, ¿debe haber respawn? Porque en 1P se empieza de cero. Esto también lo tengo que divagar y pensar.

OJU. Lo más fácil va a ser lo que más trabajo me cueste. Necesito ver opciones. Quizá tenga que hablar con estos antes de enseñar. 
BR.

20190625
========

Muchas cosas resueltas, apunto:

MODO 1P
	- Poder elegir entre Espinete y Don Pimpón.
	- Morir significa reiniciar la fase.

MODO CO-OP
	- P1 es Espinete, P2 es Don Pimpón.
	- Si P1 o P2 muere, se desactiva momentaneamente.
	- Si ambos mueren, se resta una vida.
	- Si el que queda termina el nivel, "salva" a su compañero.
	- La puntuación se suma.

MODO VS
	- Se elige una fase.
	- P1 es Espinete, P2 es Don Pimpón.
	- Si P1 o P2 muere, se desactiva momentaneamente.
	- Gana quien llegue arriba.
	- Si ambos o ninguno llega arriba, el que haya conseguido más puntos.

