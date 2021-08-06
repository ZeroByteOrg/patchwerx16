#include <stdint.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>	//abs()

#include "mouse.h"
#include "widgets.h"
#include "patchwerx16.h"

extern void wait(); // pauses until next IRQ. function is in wait.asm

void draw_screen()
{
	widget.draw[0](0);
}

// TODO: impliment a routine set_widget() to intialze the basics...
// Params are the very basics: clickbox x,y,w,h
// It should then point all handler references to null handlers;
//
// Thought: Maybe I should move clickboxes outside of the widget struct...
// Then it would be possible to have non-clickable widgets that can still be
// triggered to update their appearance or whatever...
void init_widgits()
{
	  widget.onClick[0]	= click_test;
		 widget.draw[0]	= render_test;
	 widget.onChange[0]	= mod_test;
		widget.value[0]	= 16;
		widget.color[0] = COLOR_BLUE << 4 | COLOR_WHITE;
		  widget.min[0]	= 0;
		  widget.max[0] = 31;
	 widget.vram_loc[0]	= 0x4000 + (40 + 128*4); // col 40, row 4
			widget.x[0]	= (40 * 8);
			widget.y[0]	= (4 * 8);
		   widget.x1[0]	= (40*8)+16;
		   widget.y1[0]	= (40*8)+16;
		widget.state[0]	= WS_ENABLED;
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
			c = ~ vpeek(159);
			vpoke(c,159);
			id = find_widget(click.x,click.y);
			if (id >= 0)
				widget.onClick[id](id);
		}
	}
}
