#include <stdio.h>
#include "keyboard.h"

kbd_state kbd = {};


void kbd_init()
{
	uint8_t i;
	
	for (i=0 ; i <= KBD_BUFFER_SIZE ; i++)
		kbd.buffer[i] = 0;
	kbd.pressed = 0;
	kbd.bufferlen = 0;
}
