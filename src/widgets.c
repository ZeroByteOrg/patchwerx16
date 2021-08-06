#include <cx16.h>

#include "widgets.h"

widget_set widget = {};

extern void init_widgits()
{
	  widget.onClick[0]	= click_test;
		 widget.draw[0]	= render_test;
	 widget.onChange[0]	= mod_test;
		widget.value[0]	= 69;
		widget.color[0] = COLOR_BLUE << 4 | COLOR_WHITE;
		  widget.min[0]	= 32;
		  widget.max[0] = 128;
	 widget.vram_loc[0]	= (80 + 256*4); // col 40, row 4
			widget.x[0]	= (40 * 8);
			widget.y[0]	= (4 * 8);
			widget.w[0]	= 16;
			widget.h[0]	= 16;
		widget.state[0]	= WS_ENABLED;
}

int16_t find_widget(const uint16_t x, const uint16_t y)
{
	return x+y-x-y; // just to shut up the compiler warning!!!
}

void set_widget(uint8_t id, int16_t value)
{
	// clamp the supplied value to uint_8 range
	if (value < widget.min[id])
		value = widget.min[id];
	else if (value > widget.max[id])
		value = widget.max[id];
	widget.value[id] = value;
	widget.draw[id](id);
	// TODO: update the YM with this value
}

void inc_widget(uint8_t id, uint8_t delta)
{
	set_widget(id, widget.value[0]+delta);
}

void dec_widget(uint8_t id, uint8_t delta)
{
	set_widget(id, widget.value[0]+delta);
}

