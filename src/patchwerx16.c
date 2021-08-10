#include <stdint.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>	//abs()

//#define MAXWIDGITS 3
#include "patchwerx16.h"
#include "zwidgits.h"
#include "ym2151.h"
#include "mywidgits.h"

extern void wait(); // pauses until next IRQ. function is in wait.asm

void draw_screen()
{
	widgit.draw[0](0); //<===== this is crashing the program....
}

// TODO: impliment a routine set_widgit() to intialze the basics...
// Params are the very basics: clickbox x,y,w,h
// It should then point all handler references to null handlers;
//
// Thought: Maybe I should move clickboxes outside of the widgit struct...
// Then it would be possible to have non-clickable widgits that can still be
// triggered to update their appearance or whatever...
void setup_widgits()
{
	int16_t id, box;
	id = add_widgit(&YM.voice[0].op[0].tl);
	box = add_clickbox(20*16, 4*16, 16*3, 16);
	if ((id < 0)||(box < 0)) return;
	attach_clickbox(box,id);
	widgit.draw[id] = render_test;
	widgit.color[id] = 1 << 4; // pre-shift the color palette # into
	widgit.min[id] = 0;       //proper bits for tile mode display
	widgit.max[id] = 0x7f;
	widgit.vram_loc[id] = 0x4000 + (40 + 128*4); // col 40, row 4
	widgit.state[id] = WS_ENABLED;
	widgit.l_click[id] = dec_repeat_drag;
	widgit.r_click[id] = inc_repeat_drag;
}

void debug_showclick(int16_t id)
{
	printf("click: x=%03d y=%03d b=%02x id=%d\n",click.x, click.y, click.buttons,id);
}

uint8_t patchwerx_init()
{
	widgit_sysinit();
	setup_widgits();
	draw_screen();
	return 1;
}

void program_loop()
{
	mouse_show();
	while (1)
	{
		mouse_get(); // rename to get_input()?
		
		// program gets first bite at handling clicks.
		// if it wants to consume the click and not process the widgits,
		// then just don't call widgits_process_click();
		if (click.buttons) widgits_process_click();
		if (kbd.pressed) widgits_process_key();
	}
}
