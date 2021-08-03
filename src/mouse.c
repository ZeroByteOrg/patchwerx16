#include <stddef.h> // for offsetof()
#include <stdio.h>
#include "patchwerx16.h"
#include "mouse.h"

#define ZP_RETURNS		0x40	// location for 4-byte Kernal return values
#define MOUSE_CONFIG	0xFF68	// Kernal API location
#define MOUSE_GET		0xFF6B	// Kernal API location

mouse_state mouse = {};
mouse_state click = {};

void mouse_hide()
{
	asm("lda #%b", 0);
	asm("ldx #%b", 1);
	asm("jsr %w", MOUSE_CONFIG);
}

void mouse_show()
{
	asm("lda #%b", 1);
	asm("ldx #%b", 1);
	asm("jsr %w", MOUSE_CONFIG);
}

void mouse_get()
{
	mouse.previous = mouse.buttons;
	click.buttons  = 0;	// clear previous click snapshot

	// call Kernal MOUSE_GET
	asm("ldx #%b", ZP_RETURNS);
	asm("jsr %w",  MOUSE_GET);
	// Kernal returns buttons in accumulator. Move it to the struct.
	asm("ldy #%b", offsetof(mouse_state, buttons));
	asm("sta %v,y", mouse);
	mouse.x = (*(int16_t*) ZP_RETURNS);
	mouse.y = (*(int16_t*) (ZP_RETURNS+2));
	mouse.pressed = (mouse.buttons & ~mouse.previous) & 0x07;
	if (mouse.pressed)
		click=mouse;
}

void mouse_waitrelease(const uint8_t mask)
{
	while (mouse.buttons & mask)
		mouse_get();
}

