#include <cx16.h>
#include "widgets.h"

extern void init_widgit(widget_t *widget, void *clickfunc, void *drawfunc, uint8_t *value)
{
	widget->onClick = (clickhandler)clickfunc;
	widget->draw    = (renderer)drawfunc;
	widget->value	= value;
}

widget_t *find_widget(const uint16_t x, const uint16_t y)
{
	//return (widget_t*)0x0000;

	return &widget[0];
}


void set_widget(widget_t *p_widget, int16_t value)
{
	// clamp the supplied value to uint_8 range
	if (value < 0)
		value = 0;
	else if (value > 255)
		value = 255;
	*p_widget->value = value;
	p_widget->draw(&p_widget);
	// TODO: update the YM with this value
}
