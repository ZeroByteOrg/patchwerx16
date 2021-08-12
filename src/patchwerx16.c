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
void playtone();	// delme when done - don't leave crap laying around!!!
void draw_screen()
{
	widgit.draw[0](0);
}

void setup_widgits()
{
	int16_t id, box;
	id = add_widgit(&YM.oper[0][0][YMVAL_TL]);
	box = add_clickbox(20*16, 4*16, 16*3, 16);
	if ((id < 0)||(box < 0)) return;
	attach_clickbox(box,id);
	widgit.draw[id] = knob1_draw;
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
	playtone();
	return 1;
}

void program_loop()
{
	mouse_show();
	while (1)
	{
		mouse_get(); // rename to get_input()?
		
		// program gets first bite at handling input.
		// if it wants to consume the click/key and not process the 
		// widgits, then just don't call widgits_process_x();
		if (click.buttons) widgits_process_click();
		if (kbd.pressed) widgits_process_key();
	}
}

void playtone()
{
	ym_set_param(YMVAL_L, 1, 0);
	ym_set_param(YMVAL_R, 1, 0);
	ym_set_param(YMVAL_CONECT, 7, 0);
	ym_set_param(YMVAL_FB, 0, 0);
	ym_set_param(YMVAL_TL, 0x06, 0, 0);
	ym_set_param(YMVAL_TL, 0x7f, 0, 1);
	ym_set_param(YMVAL_TL, 0x7f, 0, 2);
	ym_set_param(YMVAL_TL, 0x7f, 0, 3);
	ym_set_param(YMVAL_AR, 0x0f, 0, 0);
	ym_set_param(YMVAL_AR, 0x1f, 0, 0);
	ym_set_param(YMVAL_AR, 0x1f, 0, 1);
	ym_set_param(YMVAL_AR, 0x1f, 0, 2);
	ym_set_param(YMVAL_AR, 0x1f, 0, 3);
	ym_set_param(YMVAL_RR, 0x0f, 0, 0);
	ym_set_param(YMVAL_RR, 0x0f, 0, 1);
	ym_set_param(YMVAL_RR, 0x0f, 0, 2);
	ym_set_param(YMVAL_RR, 0x0f, 0, 3);
	ym_set_param(YMVAL_KC, 0x4a, 0);
	ym_set_param(YMVAL_KON, 0);
	ym_set_param(YMVAL_KON, 0x78);
	set_widgit(0,0x7f);
}

