#include <cx16.h>

#include "widgits.h"

#define KNOB_OFFSET	32
#define KNOB_COLOR	5 << 4


void render_test(uint8_t id)
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
