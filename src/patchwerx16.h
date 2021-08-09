#ifndef __patchwerx16_h__
#define __patchwerx16_h__

#include <stdint.h>

#define MAX_WIDGITS	80	// holy crap - 80???

extern void wait();				// pause until next IRQ
extern void draw_screen();
extern void program_loop();
extern void init_widgits();
extern uint8_t patchwerx_init();
extern void debug_showclick(int16_t id);


#endif
