#include <stdint.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>	//abs()

#include "mouse.h"
#include "widgets.h"
#include "patchwerx16.h"

#define DRAGBOX	4

widget_t widget[MAX_WIDGETS] = {};

void draw_screen()
{
	draw_widget(&widget[0]);
}

void dragedit(widget_t *p_widget,int16_t *p_axis,const int16_t p_anchor,const uint8_t p_button)
{
	//volatile int16_t delta;
	volatile int16_t newvalue;
	   const int16_t startvalue = p_widget->value;

	do {
		newvalue = startvalue + (p_anchor - *p_axis);
		set_widget(p_widget, newvalue);
		mouse_get();
		// TODO: check keyboard for ESC key to abort the edit
	} while (mouse.buttons & p_button);
}

void clickedit(widget_t *p_widget, const int8_t delta)
{
	set_widget(p_widget, (int8_t)(p_widget->value + delta));
}

void handle_click(const uint8_t p_button)
{
	static widget_t *w;
	volatile uint8_t count = 256;
	volatile int16_t x = mouse.x;
	volatile int16_t y = mouse.y;

	w = find_widget(mouse.x,mouse.y);
	if (w == (widget_t*)0x0000)
	{
		mouse_waitrelease(p_button);
		return;
	}

	while ((mouse.buttons & p_button) && count > 0)
	{
		--count;
		mouse_get();
		// if mouse leaves DRAGBOX, go into drag mode
		if (abs(mouse.x - x) > DRAGBOX)
		{
			// dragedit should run until mouse button is released
			dragedit(w,&mouse.x,x,p_button);
			count = 1; // ensure count >0 so we don't also process a click edit
		}
		else if (abs(mouse.y - y) > DRAGBOX)
		{
			dragedit(w,&mouse.y,y,p_button);
			count = 1;
		}
	}
	if (count==0)
		clickedit(w,p_button == MBUTTON_L ? -1 : 1);
}

void program_loop()
{
	mouse_show();
	while (1)
	{
		mouse_get();
		if (mouse.pressed & MBUTTON_L)
			handle_click(MBUTTON_L);
		else if (mouse.pressed & MBUTTON_R)
			handle_click(MBUTTON_R);
		else if (mouse.pressed & MBUTTON_M)
			return;
	}
}
