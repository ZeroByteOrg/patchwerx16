#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

#include <stdint.h>


#define CB_TYPE	int8_t

typedef CB_TYPE (*cb_clickhandler)(uint8_t);
	// param = widget index

typedef CB_TYPE (*cb_renderer)(uint8_t);
	// param = widget index

typedef CB_TYPE (*cb_action)(uint8_t,int16_t);
	// params = widget index, previous value)


// click handler declarations:
extern CB_TYPE click_test(uint8_t id);

// renderers declarations:
extern CB_TYPE render_test(uint8_t id);

// action trigger declarations:
extern CB_TYPE mod_test(uint8_t id, int16_t oldvalue);

#endif
