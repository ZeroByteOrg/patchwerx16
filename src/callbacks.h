#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

#include <stdint.h>


#define CB_TYPE	int8_t

// click handler declarations:
extern void inc_repeat_drag(uint8_t id);
extern void dec_repeat_drag(uint8_t id);

// renderers declarations:
extern void render_test(uint8_t id);

// action trigger declarations:
extern CB_TYPE mod_test(uint8_t id, int16_t oldvalue);

#endif
