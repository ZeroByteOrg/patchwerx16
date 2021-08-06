#include <stdint.h>
#include <cx16.h>
#include <stdlib.h>	//abs()

#include "patchwerx16.h"
#include "callbacks.h"
#include "widgets.h"
#include "mouse.h"


#define DRAGBOX	4
#define CLICK_DELAY1	45
#define CLICK_DELAY2	7

void dragedit(uint8_t id,int16_t *p_delta,const uint8_t p_button)
{
	const int16_t startvalue = widget.value[id];

	do {
		set_widget(id, widget.value[id] + *p_delta);
		mouse_get();
		#if(0)
		// TODO: check keyboard for ESC key to abort the edit
		if (0) // if new-key-pressed == ESC
		{
			set_widget(id, startvalue);
			return;
		}
		#endif
	} while (mouse.buttons & p_button);
}

CB_TYPE click_test(uint8_t id)
{
	static mouse_click m_click;
	static void (*f)(uint8_t, uint8_t);
	volatile int16_t count = 0;
	volatile uint8_t first = 1;
	volatile uint8_t draggable = 1;
	m_click = click; // take snapshot of global click struct
	if (m_click.buttons == MBUTTON_L) f = dec_widget;
	  else f = inc_widget;
	do {
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
			f(id,1);
		}
		else wait();
	} while (mouse.buttons & m_click.buttons);
	return (CB_TYPE)0;
}




CB_TYPE render_test(uint8_t id)
{
	/*
	vpoke(widget.value[id],widget.vram_loc[id]);
	vpoke(widget.value[id],widget.vram_loc[id]+2);
	vpoke(widget.value[id],widget.vram_loc[id]+256);
	vpoke(widget.value[id],widget.vram_loc[id]+256 + 2);
	*/
	vpoke(widget.value[id],widget.vram_loc[id]);
	return (CB_TYPE)0;
}

CB_TYPE mod_test(uint8_t id, int16_t oldvalue)
{
	id = oldvalue; // just to shut up the compiler warnings....
	return (CB_TYPE)0;
}

