#ifndef __patchwerx16_h__
#define __patchwerx16_h__

#include <cx16.h>

extern uint8_t testvalue;

extern void wait();				// pause until next IRQ
extern void draw_screen();
extern void program_loop();


#endif
