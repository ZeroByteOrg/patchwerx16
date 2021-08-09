#ifndef __HANDLERS_H__
#define __HANDLERS_H__

#include <stdint.h>

/* Master include file for all handler types. The definitions are
 * broken into seperate files since there are probably going to be
 * several of each type of handler.
 * 
 * These are the "per-project" definitions, and are defined separate
 * from the main widgits 'library' (I intend to flesh this out into a
 * standalone general-purpose widgits library for later reuse)
*/


// click handler declarations:
extern void inc_repeat_drag(uint8_t id);
extern void dec_repeat_drag(uint8_t id);

// key handler declarations:

// renderers declarations:
extern void render_test(uint8_t id);

#endif
