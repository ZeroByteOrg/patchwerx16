#ifndef __MOUSE_H__
#define __MOUSE_H__

#include <stdint.h>

#define MBUTTON_L	1
#define MBUTTON_R	2
#define MBUTTON_M	4

typedef struct mouse_state {
	int16_t	x, y;
	uint8_t	buttons,	// current button states
			previous,	// previous button states
			pressed;	// buttons pressed on this check
} mouse_state;

extern mouse_state mouse;

extern void mouse_hide();
extern void mouse_show();
extern void mouse_get();
extern void mouse_waitrelease(const uint8_t mask);

#endif
