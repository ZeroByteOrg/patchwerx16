#ifndef __WIDGETS_H__
#define __WIDGETS_H__

#include <stdint.h>
#include "ym2151.h"

#ifndef MAX_WIDGETS
#define MAX_WIDGETS	2
#endif

enum widget_state {
	WS_DISABLED,
	WS_ENABLED,
	WS_SELECTED
};

enum widget_style {
	W_KNOB_1DIGITR,
	W_KNOB_2DIGITR,
	W_SLIDER_VERT,
	W_SLIDER_HORIZ
};

typedef struct widget_t {
	enum ym_param		param;
	enum widget_state	state;
	enum widget_style	style;
	uint8_t color;
	uint16_t vram_loc;
	uint8_t value, min_value, max_value;
	void *set_param();

} widget_t;

extern widget_t widget[MAX_WIDGETS];

extern widget_t *find_widget(const uint16_t x, const uint16_t y);
extern void draw_widget(widget_t *p_widget);
extern void set_widget(widget_t *p_widget, int16_t value);

#endif
