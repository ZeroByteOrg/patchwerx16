/*
 * main.c
 *
 * Copyright 2021 Barry Yost <thatguy@zerobyte.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

//#include <stdio.h>
#include <stdint.h>
#include <conio.h>		// kbhit() cgetc()
//#include <cx16.h>
#include "patchwerx16.h"
#include "widgets.h"

void init_widgits();

typedef struct x16_state {
	uint8_t	vera1;
} x16_state;

uint8_t system_init()
{
	// TODO: setup VERA
	ym_init();
	init_widgits();
	draw_screen();
	return 1;
}

void init_widgits()
{
	widget[0].style = W_KNOB_1DIGITR;
	widget[0].value = 32;
	widget[0].color = 0x41;
	widget[0].vram_loc	= 0; //( 2*40 + 256*20 );
}

void system_shutdown()
{
	ym_silence(0xff); // values not 0-7 = silence all.
	// clear the key buffer
	while (kbhit())
		cgetc();
	// wait for keypress prior to exit
	// (cc65's underlying code clears the screen when done - grr)
	while (!kbhit()) {}
}

void system_get_state(x16_state *p_state)
{
	volatile x16_state state;

	state = *p_state;

	state.vera1 = 1;
}

void system_restore_state(x16_state *p_state)
{
	uint8_t x;
	volatile x16_state state;
	state = *p_state;
	x = state.vera1;
}

int main()
{
	x16_state state;

	system_get_state(&state);
	if (system_init())
		program_loop();
	system_shutdown();
	system_restore_state(&state);
	return 0;
}
