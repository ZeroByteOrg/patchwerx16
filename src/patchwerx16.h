#ifndef __patchwerx16_h__
#define __patchwerx16_h__

#include <stdint.h>

#define MAX_WIDGETS	80	// holy crap - 80???

extern void wait();				// pause until next IRQ
extern void draw_screen();
extern void program_loop();
extern uint8_t patchwerx_init();


#endif
