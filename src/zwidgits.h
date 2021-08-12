#ifndef __ZWIDGITS_H__
#define __ZWIDGITS_H__

/*
 * Ideas for this library's enhancements:
 * 
 * - to get rid of #maxwidgits, turn it into an array of widgit* instead
 *   of the current struct-of-arrays data dump.
 * 
 * - widget classes
 * 		- zw_register_class(class_id, signed_int *initializer() )
 * 		- allows user-created widget types
 * 
 * - data moved into external structs
 * 
 * - abandon the widgit id# system in favor of passing pointers
 * 
 * - do the callback functions even need pointers to the widgit
 *   that called them? I think so - makes it less cumbersome to return
 *   exit codes that signal whether or not to refresh / call trigger() etc...
 * 
 * - get head out of clouds for this particular project though!!!!!
 * 
 * -- see above statement
*/
	
#include <stdint.h>
#include "zw_mouse.h"
#include "zw_keyboard.h"

#include "patchwerx16.h" // defines MAX_WIDGITS to actual value
// KILL THIS WITH FIRE!!!!
// zwidgits should NOT need ANY info from
// the project it's a part of!!!


#ifndef MAX_WIDGITS
#define MAX_WIDGITS	4
#endif

enum zw_callback_types {
	CB_CLICK,
	CB_KEY,
	CB_RENDER,
	CB_TRIGGER,
	CB_numcallbacks
};

typedef void* cb_set[CB_numcallbacks];

typedef enum widgit_state {
	WS_HIDDEN,
	WS_DISABLED,
	WS_ENABLED,
	WS_SELECTED,
	WS_CUSTOM
} widgit_state;

// am I barking up the wrong tree with this?
enum widgit_type {
	WT_RANGE,
	WT_CYCLE,
	WT_TOGGLE,
	WT_RADIO,
	WT_TEXT
};

typedef void (*cb_clickhandler)(uint8_t);
	// param = widgit index

typedef void (*cb_renderer)(uint8_t);
	// param = widgit index
	
typedef void (*cb_keyhandler)(uint8_t);
	// param = widgit index

typedef void (*cb_trigger)(uint8_t);
	// param = widgit index

typedef struct widgit_set {
	enum widgit_state state[MAX_WIDGITS];
	void *value[MAX_WIDGITS];
	uint8_t min[MAX_WIDGITS];
	uint8_t max[MAX_WIDGITS];
	uint8_t color[MAX_WIDGITS];
	uint16_t vram_loc[MAX_WIDGITS];
	cb_set* cb[MAX_WIDGITS];
	uint8_t clickbox[MAX_WIDGITS];
	uint8_t count;
} widgit_set;

typedef struct clickbox_set {
	uint16_t x[MAX_WIDGITS];		// left of the clickbox
	uint16_t x1[MAX_WIDGITS];		// right of the clickbox
	uint16_t y[MAX_WIDGITS];		// top of the clickbox
	uint16_t y1[MAX_WIDGITS];		// bottom of the clickbox
	int16_t	widgit[MAX_WIDGITS];	// id of widgit this clickbox calls
	uint8_t	count;
} clickbox_set;

extern widgit_set widgit;
extern clickbox_set clickbox;

extern void widgit_sysinit();
extern uint16_t add_widgit(uint8_t *val);
extern uint16_t add_clickbox(const uint16_t x, const uint16_t y, 
						    const uint16_t w, const uint16_t h);
extern void attach_clickbox (uint8_t boxid, uint8_t widgitid);
//extern int16_t find_widget(const uint16_t x, const uint16_t y);
extern int16_t widgits_process_click();
extern int16_t widgits_process_key();
extern void set_widgit(uint8_t id, int16_t value);

#endif
