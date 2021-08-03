#include <stdint.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>	//abs()

#include "mouse.h"
#include "widgets.h"
#include "patchwerx16.h"

#define DRAGBOX	4
#define CLICK_DELAY1	45
#define CLICK_DELAY2	7

extern void wait(); // pauses until next IRQ. function is in wait.asm

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
		// doesn't it suck that we don't have direct dX/dY mouse reporting
		// from the Kernal?
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

void handle_click()
{
	static widget_t *w;
	static mouse_state m_click;
	volatile int16_t count = 0;
	volatile uint8_t first = 1;
	volatile uint8_t draggable = 1;

	w = find_widget(m_click.x,m_click.y);
	if (w == (widget_t*)0x0000)
		return;
	m_click = click;

	// filter out multi-clicks, priority = Left, Right, Middle
	if (m_click.buttons & MBUTTON_L)
		m_click.buttons = MBUTTON_L;
	else if (m_click.buttons & MBUTTON_R)
		m_click.buttons = MBUTTON_R;
	else
		m_click.buttons = MBUTTON_M;

	do {
		mouse_get();
		// if mouse leaves DRAGBOX, go into drag mode
		if ((abs(mouse.x - m_click.x) > DRAGBOX) && draggable)
		{
			// dragedit should run until mouse button is released
			dragedit(w,&mouse.x,m_click.x,m_click.buttons);
			break;
		}
		else if ((abs(mouse.y - m_click.y) > DRAGBOX) && draggable)
		{
			dragedit(w,&mouse.y,m_click.y,m_click.buttons);
			break;
		}
		--count;
		if (count < 0)
		{
			if (first)
			{
				first = 0;
				count = CLICK_DELAY1;
			}
			else
			{
				draggable = 0;
				count = CLICK_DELAY2;
			}
			clickedit(w,m_click.buttons == MBUTTON_L ? -1 : 1);
		}
		else
		{
			wait();
		}
	} while (mouse.buttons & m_click.buttons);
}

void program_loop()
{
	uint8_t c;
	mouse_show();
	while (1)
	{
		mouse_get();
		if (click.buttons)
		{
			c = ~ vpeek(159);
			vpoke(c,159);
			handle_click();
		}
	}
}
