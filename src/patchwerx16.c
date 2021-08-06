#include <stdint.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>	//abs()

#include "mouse.h"
#include "widgets.h"
#include "patchwerx16.h"

extern void wait(); // pauses until next IRQ. function is in wait.asm

void draw_screen()
{
	widget.draw[0](0);
}

/*
void clickedit(widget_t *p_widget, const int8_t delta)
{
	set_widget(p_widget, (int8_t)(p_widget->value + delta));
}
*/

void program_loop()
{

	static uint8_t c;
	static int16_t id;
	mouse_show();
	while (1)
	{
		mouse_get();
		if (click.buttons)
		{
			c = ~ vpeek(159);
			vpoke(c,159);
			id = find_widget(click.x,click.y);
			if (id >= 0)
				widget.onClick[id](id);
		}
	}
}
