#include <cx16.h>

#include "widgets.h"

widget_set widget = {};

int16_t find_widget(const uint16_t x, const uint16_t y)
{
	if ((x >= widget.x[0]) && (x <= widget.x1[0]) &&
			(y >= widget.y[0]) && (y <= widget.y1[0]))
	{
		return 0;
	}
	
	return -1;
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
	set_widget(id, widget.value[0]-delta);
}

