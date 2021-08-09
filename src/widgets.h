#ifndef __WIDGETS_H__
#define __WIDGETS_H__

#include <stdint.h>
#include "patchwerx16.h" // defines MAX_WIDGETS to actual value
#include "callbacks.h"

#ifndef MAX_WIDGETS
#define MAX_WIDGETS	2
#endif


typedef enum widget_state {
	WS_HIDDEN,
	WS_DISABLED,
	WS_ENABLED,
	WS_SELECTED,
	WS_CUSTOM
} widget_state;

// am I barking up the wrong tree with this?
enum widget_type {
	WT_RANGE,
	WT_CYCLE,
	WT_TOGGLE,
	WT_RADIO,
	WT_TEXT
};

typedef void (*cb_clickhandler)(uint8_t);
	// param = widget index

typedef void (*cb_renderer)(uint8_t);
	// param = widget index

typedef struct widget_set {
	enum widget_state state[MAX_WIDGETS];
	void *value[MAX_WIDGETS];
	uint8_t color[MAX_WIDGETS];
	uint16_t vram_loc[MAX_WIDGETS];
	uint8_t min[MAX_WIDGETS];
	uint8_t max[MAX_WIDGETS];
	cb_renderer draw[MAX_WIDGETS];
	cb_clickhandler l_click[MAX_WIDGETS];
	cb_clickhandler r_click[MAX_WIDGETS];
	cb_clickhandler m_click[MAX_WIDGETS];
	uint8_t clickbox[MAX_WIDGETS];
	uint8_t count;
} widget_set;

typedef struct clickbox_set {
	uint16_t x[MAX_WIDGETS];	// left of the clickbox
	uint16_t x1[MAX_WIDGETS];	// right of the clickbox
	uint16_t y[MAX_WIDGETS];	// top of the clickbox
	uint16_t y1[MAX_WIDGETS];	// bottom of the clickbox
	uint8_t	widget[MAX_WIDGETS];				// id of widget this clickbox calls
	uint8_t	count;
} clickbox_set;

extern widget_set widget;
extern clickbox_set clickbox;

extern uint16_t add_widgit(uint8_t *val);
extern uint16_t add_clickbox(const uint16_t x, const uint16_t y, 
						    const uint16_t w, const uint16_t h);
extern void attach_clickbox (uint8_t boxid, uint8_t widgetid);
extern int16_t get_clickbox(const uint16_t x, const uint16_t y);
extern void set_widget(uint8_t id, int16_t value);
extern void inc_widget(uint8_t id, uint8_t delta);
extern void dec_widget(uint8_t id, uint8_t delta);

#endif
