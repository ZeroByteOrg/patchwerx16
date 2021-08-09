#include <cx16.h>

#include "widgets.h"

widget_set widget = {};
clickbox_set clickbox = {};

void null_clickhandler(uint8_t id)
{
}

void null_draw(uint8_t id)
{
}


uint16_t add_widgit(uint8_t *val)
{
	uint8_t id = widget.count+1;
	if (id > MAX_WIDGETS)
		return -1;
	widget.count = id;
	widget.value[id] = val;
	widget.draw[id] = null_draw;
	widget.l_click[id] = null_clickhandler;
	widget.r_click[id] = null_clickhandler;
	widget.m_click[id] = null_clickhandler;
	widget.clickbox[id] = MAX_WIDGETS;
	return id;
}

extern uint16_t add_clickbox(const uint16_t x, const uint16_t y, 
						    const uint16_t w, const uint16_t h)
{
	uint8_t id = clickbox.count+1;
	if (id == MAX_WIDGETS)
		return -1;
	++clickbox.count;
	clickbox.x[id] = x;
	clickbox.y[id] = y;
	clickbox.x1[id] = x + w;
	clickbox.y1[id] = y + h;
	return id;
}

extern void attach_clickbox(uint8_t boxid, uint8_t widgetid)
{
	clickbox.widget[boxid] = widgetid;
	widget.clickbox[widgetid] = boxid;
}



// for now - just called when a click happens. Might make more sense to keep the
// clickbox array sorted somehow, and keep a running track on which clickbox
// the mouse is in at all times...?
int16_t get_clickbox(const uint16_t x, const uint16_t y)
{
	uint8_t i;
	for (i = 0 ; i<clickbox.count ; i++)
	{
		if ((x >= clickbox.x[i]) && (x <= clickbox.x1[i]) &&
				(y >= clickbox.y[i]) && (y <= clickbox.y1[i]))
		{
			return (uint16_t)i;
		}
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
	*(uint8_t*)widget.value[id] = value;
	widget.draw[id](id);
	// TODO: update the YM with this value
}

void inc_widget(uint8_t id, uint8_t delta)
{
	set_widget(id, *(uint8_t*)widget.value[0]+delta);
}

void dec_widget(uint8_t id, uint8_t delta)
{
	set_widget(id, *(uint8_t*)widget.value[0]-delta);
}

