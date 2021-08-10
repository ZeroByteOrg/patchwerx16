#ifndef __ZWIDGITS_H__
#define __ZWIDGITS_H__

#include <stdint.h>
#include "zw_mouse.h"
#include "zw_keyboard.h"

// KILL THIS WITH FIRE!!!!
#include "patchwerx16.h" // defines MAX_WIDGITS to actual value


#ifndef MAX_WIDGITS
#define MAX_WIDGITS	1
#endif


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

typedef struct widgit_set {
	enum widgit_state state[MAX_WIDGITS];
	void *value[MAX_WIDGITS];
	uint8_t color[MAX_WIDGITS];
	uint16_t vram_loc[MAX_WIDGITS];
	uint8_t min[MAX_WIDGITS];
	uint8_t max[MAX_WIDGITS];
	cb_renderer draw[MAX_WIDGITS];
	cb_clickhandler l_click[MAX_WIDGITS];
	cb_clickhandler r_click[MAX_WIDGITS];
	cb_clickhandler m_click[MAX_WIDGITS];
	cb_keyhandler keypress[MAX_WIDGITS];
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
//extern void inc_widgit(uint8_t id, uint8_t delta);
//extern void dec_widgit(uint8_t id, uint8_t delta);

#endif
