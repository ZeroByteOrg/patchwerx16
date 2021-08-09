#include <stdint.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>	//abs()

//#define MAXWIDGETS 3
#include "widgets.h"
#include "patchwerx16.h"
#include "ym2151.h"
#include "callbacks.h"

extern void wait(); // pauses until next IRQ. function is in wait.asm

void draw_screen()
{
	widget.draw[0](0); //<===== this is crashing the program....
}

// TODO: impliment a routine set_widget() to intialze the basics...
// Params are the very basics: clickbox x,y,w,h
// It should then point all handler references to null handlers;
//
// Thought: Maybe I should move clickboxes outside of the widget struct...
// Then it would be possible to have non-clickable widgets that can still be
// triggered to update their appearance or whatever...
void init_widgets()
{
	int16_t id, box;
	widget.count = 0;
	clickbox.count = 0;
	id = add_widgit(&YM.voice[0].op[0].tl);
	box = add_clickbox(40*8, 4*8, 16, 16);
	if ((id < 0)||(box < 0)) return;
	attach_clickbox(box,id);
	widget.draw[id] = &render_test;
	widget.color[id] = 1 << 4; // pre-shift the color palette # into
	widget.min[id] = 0;       //proper bits for tile mode display
	widget.max[id] = 0x7f;
	widget.vram_loc[id] = 0x4000 + (40 + 128*4); // col 40, row 4
	widget.state[id] = WS_ENABLED;
	widget.l_click[id] = dec_repeat_drag;
	widget.r_click[id] = inc_repeat_drag;
}

uint8_t patchwerx_init()
{
	return 1;
}

void program_loop()
{

	static uint8_t c;
	static int16_t id;
	mouse_show();
	while (1)
	{
		mouse_get();
		if (click.buttons)
		{
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
			id = get_clickbox(click.x,click.y);
			if (id >= 0)
				switch (click.buttons) {
					case MBUTTON_L:
						widget.l_click[id];
						break;
					case MBUTTON_R:
						widget.r_click[id];
						break;
					case MBUTTON_M:	
						widget.m_click[id];
						break;
				}
		}
	}
}
