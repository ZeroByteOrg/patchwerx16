#include <cx16.h> // vpeek() vpoke()

#include "widgits.h"

widgit_set widgit = {};
clickbox_set clickbox = {};

#pragma warn (unused-param,push,off)
void null_clickhandler(uint8_t id)
{
}

void null_draw(uint8_t id)
{
}

void null_keyhandler(uint8_t id)
{
}
#pragma warn (unused-param,pop) 

void widgit_sysinit()
{
	kbd_init();
	mouse_init();
	widgit.count = 0;
	clickbox.count = 0;
}

uint16_t add_widgit(uint8_t *val)
{
	static uint8_t id;
	id = widgit.count;
	if (id >= MAX_WIDGITS)
		return -1;
	++widgit.count;
	widgit.value[id] = val;
	widgit.draw[id] = null_draw;
	widgit.l_click[id] = null_clickhandler;
	widgit.r_click[id] = null_clickhandler;
	widgit.m_click[id] = null_clickhandler;
	widgit.keypress[id] = null_keyhandler;
	widgit.clickbox[id] = MAX_WIDGITS;
	return id;
}

extern uint16_t add_clickbox(const uint16_t x, const uint16_t y, 
						    const uint16_t w, const uint16_t h)
{
	static uint8_t id;
	id = clickbox.count;
	if (id >= MAX_WIDGITS)
		return -1;
	++clickbox.count;
	clickbox.x[id] = x;
	clickbox.y[id] = y;
	clickbox.x1[id] = x + w;
	clickbox.y1[id] = y + h;
	clickbox.widgit[id] = -1; // initialize clickbox to point at no widget
	return id;
}

extern void attach_clickbox(uint8_t boxid, uint8_t widgitid)
{
	clickbox.widgit[boxid] = widgitid;
	widgit.clickbox[widgitid] = boxid;
}



// for now - just called when a click happens. Might make more sense to keep the
// clickbox array sorted somehow, and keep a running track on which clickbox
// the mouse is in at all times...?
int16_t find_widget(const uint16_t x, const uint16_t y)
{
	uint8_t i;
	for (i = 0 ; i<clickbox.count ; i++)
	{
		if ((x >= clickbox.x[i]) && (x <= clickbox.x1[i]) &&
				(y >= clickbox.y[i]) && (y <= clickbox.y1[i]))
		{
			return clickbox.widgit[i];
		}
	}
	return -1;
}

extern int16_t widgits_process_click()
{
	static int16_t id;
	static int8_t c;
	// filter out multi-clicks, priority = L > R > M
	if (click.buttons & MBUTTON_L)
		click.buttons = MBUTTON_L;
	else if (click.buttons & MBUTTON_R)
		click.buttons = MBUTTON_R;
	else
		click.buttons = MBUTTON_M;
	// tmp: toggle the top-right char's color to indicate clicks...
	c = ~ vpeek(159);
	vpoke(c,159);
	id = find_widget(click.x,click.y);
	if (id >= 0 && id < MAX_WIDGITS)
	{
		c = ~ vpeek(155);
		vpoke(c,155);
		if (widgit.state[id] != WS_DISABLED)
			switch (click.buttons) {
				case MBUTTON_L:
					widgit.l_click[id](id);
					break;
				case MBUTTON_R:
					widgit.r_click[id](id);
					break;
				case MBUTTON_M:	
					widgit.m_click[id](id);
					break;
			}
	}
	return id;
}

int16_t widgits_process_key()
{
	return -1;
}

void set_widgit(uint8_t id, int16_t value)
{
	// clamp the supplied value to uint_8 range
	if (value < widgit.min[id])
		value = widgit.min[id];
	else if (value > widgit.max[id])
		value = widgit.max[id];
	*(uint8_t*)widgit.value[id] = value;
	widgit.draw[id](id);
	// TODO: update the YM with this value
}

#if(0)
void inc_widgit(uint8_t id, uint8_t delta)
{
	set_widgit(id, *(uint8_t*)widgit.value[0]+delta);
}

void dec_widgit(uint8_t id, uint8_t delta)
{
	set_widgit(id, *(uint8_t*)widgit.value[0]-delta);
}
#endif
