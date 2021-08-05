#ifndef __WIDGETS_H__
#define __WIDGETS_H__

#include <stdint.h>
#include "mouse.h"

#ifndef MAX_WIDGETS
#define MAX_WIDGETS	2
#endif

enum widget_state {
	WS_DISABLED,
	WS_ENABLED,
	WS_SELECTED
};

// am I barking up the wrong tree with this?
enum widget_type {
	WT_RANGE,
	WT_CYCLE,
	WT_TOGGLE,
	WT_RADIO,
	WT_TEXT
};

typedef void (*clickhandler)(void*,void*,uint8_t *);
typedef void (*renderer)(void*);

typedef struct widget_t {
	enum widget_state state;
	uint8_t *value;
	uint8_t color;
	uint16_t vram_loc;
	uint8_t min, max;
	clickhandler onClick;
	renderer draw;
} widget_t;

extern widget_t widget[MAX_WIDGETS];

extern void init_widgit(widget_t *widget, void *clickfunc, void *drawfunc, uint8_t *value);
extern widget_t *find_widget(const uint16_t x, const uint16_t y);
extern void set_widget(widget_t *p_widget, int16_t value);

#endif
