#include <stdint.h>
#include <cx16.h>
#include "callbacks.h"
#include "widgets.h"


void test_draw(widget_t *p_widget)
{
	//volatile widget_t w;
	//w = *(widget_t*)p_widget;
	vpoke(*p_widget->value,p_widget->vram_loc);
}


void draw_knob(widget_t *p_widget)
{
	vpoke(*(p_widget->value),p_widget->vram_loc);
}
