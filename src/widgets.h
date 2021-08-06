#ifndef __WIDGETS_H__
#define __WIDGETS_H__

#include <stdint.h>
#include "callbacks.h"

#ifndef MAX_WIDGETS
#define MAX_WIDGETS	2
#endif

typedef enum widget_state {
	WS_DISABLED,
	WS_ENABLED,
	WS_SELECTED
} widget_state;

// am I barking up the wrong tree with this?
enum widget_type {
	WT_RANGE,
	WT_CYCLE,
	WT_TOGGLE,
	WT_RADIO,
	WT_TEXT
};

typedef struct widget_set {
	enum widget_state state[MAX_WIDGETS];
	uint8_t value[MAX_WIDGETS];
	uint8_t color[MAX_WIDGETS];
	uint16_t vram_loc[MAX_WIDGETS];
	uint16_t x[MAX_WIDGETS];
	uint16_t w[MAX_WIDGETS];
	uint16_t y[MAX_WIDGETS];
	uint16_t h[MAX_WIDGETS];
	uint8_t min[MAX_WIDGETS];
	uint8_t max[MAX_WIDGETS];
	cb_clickhandler onClick[MAX_WIDGETS];
	cb_renderer draw[MAX_WIDGETS];
	cb_action onChange[MAX_WIDGETS];
		// meh.... ^--this should essentially do something to write
		// to the YM, or cause an action of some sort. It might even
		// not need to be definable per-widget... (this ain't C++)
} widget_set;

extern widget_set widget;

extern void init_widgits();
extern int16_t find_widget(const uint16_t x, const uint16_t y);
extern void set_widget(uint8_t id, int16_t value);
extern void inc_widget(uint8_t id, uint8_t delta);
extern void dec_widget(uint8_t id, uint8_t delta);

#endif
