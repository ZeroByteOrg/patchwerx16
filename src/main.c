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
#include <cx16.h>		// kernal access functions

#include "patchwerx16.h"
#include "widgets.h"
#include "ym2151.h"
#include "callbacks.h"


typedef struct x16_state {
	uint8_t	vera1;
} x16_state;

void vload(const char* filename, const uint8_t bank, const uint16_t address)
{
	cbm_k_setnam(filename);
	cbm_k_setlfs(0,8,0);
	cbm_k_load(bank+2,address); // bank+2 is a special functionality of X16 LOAD
}

void init_vera()
{
	uint16_t i;
	
 	cbm_k_bsout(CH_FONT_UPPER); 	//undo mixed-case mode that cc65 sets by default

	// Apply Commander X16 default video settings to layer1
	VERA.layer0.config = 0x60;
	VERA.layer0.mapbase = 0x00;
	VERA.layer0.tilebase = 0x7c;
	VERA.layer0.hscroll = 0;
	VERA.layer0.vscroll = 0;

	VERA.control = 0;   // make sure DCSEL=0 for the following writes
	VERA.display.video = 0x11;  // disable layer1 & sprites while configuring L1
	VERA.display.hscale = 0x80; // ensure display = 640x480
	VERA.display.vscale = 0x80;
	
	VERA.control = 2;	// swicht DCSEL to 1 for access to these regs
	VERA.display.hstart = 0;	// ensure full-screen drawing
	VERA.display.hstop = 0xa0;
	VERA.display.vstart = 0;
	VERA.display.vstop = 0xf0;
	
	VERA.control = 0;	// return DCSEL to 0 for access to VERA.video
	
	// configure layer 1 for tile mode 16x16 4bpp
	// tilebase = 0x10000
	// mapbase  = 0x04000
	// mapsize  = 64x32
	VERA.layer1.config = 0x12;
	VERA.layer1.mapbase = 0x20; 
	VERA.layer1.tilebase = 0x83;
	VERA.layer1.hscroll = 0;
	VERA.layer1.vscroll = 0;

	// clear the L1 screen map region
	VERA.address = 0x4000;
	VERA.address_hi = 0x10; // bank 0, stride = 1
	for (i=0 ; i < (64*32) ; i++)
	{
		VERA.data0 = 31; //todo - make this a #define from the tileset
		VERA.data0 = 0x50; // default it to use knob color palette
	}
	
	// clear the L1 tilebase region (first 64 tiles' worth should do)
	VERA.address = 0x0000;
	VERA.address_hi = 0x11;
	for (i=0 ; i < (64 * 16*16 / 2) ; i++)
		VERA.data0 = 0;
		
	VERA.display.video = 0x71;
}

uint8_t system_init()
{
	asm("sei");
	init_vera();
	asm("cli");
	
	// load in the assets....
	vload("gfx.bin",1,0);			//load widget graphics to VRAM:0x10000 
	vload("palette.bin",1,0xfa00);	//load palette
	ym_init();
	init_widgets();
	draw_screen();
	return 1;
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
