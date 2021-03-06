#include <stdint.h>
#include <stdlib.h>	//abs()
#include <cx16.h>
#include <string.h> //memcpy()
//#include <stdio.h>

#include "patchwerx16.h"
#include "zwidgits.h"
#include "mywidgits.h"
#include "ym2151.h"


#define DRAGBOX	4
#define CLICK_DELAY1	45
#define CLICK_DELAY2	7

#define KNOB_OFFSET	32
#define KNOB_COLOR	5 << 4

//void *memcpy(mouse_click *dst, mouse_click *src, size_t);

void dragedit(uint8_t id,int16_t *p_delta,const uint8_t p_button)
{
	const int16_t startvalue = *(int16_t*)widgit.value[id];

	do {
		set_widgit(id, *(uint8_t*)widgit.value[id] + *p_delta);
		mouse_get();
		#if(0)
		// TODO: check keyboard for ESC key to abort the edit
		if (0) // if new-key-pressed == ESC
		{
			set_widgit(id, startvalue);
			return;
		}
		#endif
	} while (mouse.buttons & p_button);
}

void click_test(uint8_t id, int8_t delta)
{
	static mouse_click m_click;
	int16_t	val;
	volatile int16_t count = 0;
	volatile uint8_t first = 1;
	volatile uint8_t draggable = 1;
	// take snapshot of global click struct
	memcpy(&m_click, &click, sizeof(click));
	
	do {
		val = *(uint8_t*)widgit.value[id];
		mouse_get();
		// if mouse leaves DRAGBOX, go into drag mode
		if (draggable) {
			if (abs(mouse.x - m_click.x) > DRAGBOX)	{
				// dragedit should run until mouse button is released
				dragedit(id,&mouse.dx,m_click.buttons);
				break;
			}
			else if (abs(mouse.y - m_click.y) > DRAGBOX) {
				dragedit(id,&mouse.dy,m_click.buttons);
				break;
			}
		}
		--count;
		if (count < 0) {
			if (first) {
				first = 0;
				count = CLICK_DELAY1;
			}
			else {
				draggable = 0;
				count = CLICK_DELAY2;
			}
			val += delta;
			set_widgit(id, val);  // set_widgit clamps value (for now)
		}
		else wait();
	} while (mouse.buttons & m_click.buttons);
}

void inc_repeat_drag(uint8_t id)
{
	click_test(id, 1);
}

void dec_repeat_drag(uint8_t id)
{
	click_test(id, -1);
}

void knob1_draw(uint8_t id)
{
	uint8_t color = widgit.color[id];
	uint8_t value = *(uint8_t*)widgit.value[id];
	uint8_t range = widgit.max[id]-widgit.min[id];
	int8_t knobscale = 0;
	

	//while (range & 0xc0)    { range >> 1 ; --knobscale; }
	//while (!(range & 0x30)) { range << 1 ; knobscale++; }
	
	VERA.control = 0; // point data0 at widgit's character byte
	VERA.address = widgit.vram_loc[id];
	VERA.address_hi = VERA_INC_1;
	VERA.data0 = KNOB_OFFSET + (value >> 2); // draw knob tile
	VERA.data0 = KNOB_COLOR;
	VERA.data0 = value >> 4; // high hex nybble
	VERA.data0 = color;
	VERA.data0 = value & 0x0f; // low hex nybble
	VERA.data0 = color;
}
