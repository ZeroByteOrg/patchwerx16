#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <stdint.h>

#define KBD_BUFFER_SIZE	16

typedef struct kbd_state {
	int16_t	pressed;
	uint8_t bufferlen;
	char buffer[KBD_BUFFER_SIZE+1];
} kbd_state;

extern kbd_state kbd;

void kbd_init();
#endif
