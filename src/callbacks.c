#include <stdint.h>
#include "callbacks.h"


void test_draw(widget_t *p_widget)
{
	vpoke(*(p_widget->value),p_widget->vram_loc);
}


void draw_knob(widget_t *p_widget)
{
	vpoke(*(p_widget->value),p_widget->vram_loc);
}
