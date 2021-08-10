#ifndef __MYWIDGETS_H__
#define __MYWIDGETS_H__

#include <stdint.h>

/* Master include file for all widgit implementation code files
 * e.g.: knob1.c functions should have its exported headers here
 * 
 * This is just to cut down on the number of .h files - if it starts
 * to get ugly, will split into separate .h files later
 * 
 * The design is for each widgit module to #include this file for
 * common functions like typical click handlers, etc. They may
 * #define __COMMON_ONLY__ to suppresss the declarations of the
 * other widgits' functions.
*/


#ifndef __COMMON_ONLY__

// knob1 functions
extern void inc_repeat_drag(uint8_t id);
extern void dec_repeat_drag(uint8_t id);
extern void render_test(uint8_t id);
#endif // __COMMON_ONLY__

#endif
