#include <stdio.h>
#include <stdint.h>
#include "ym2151.c"



int main()
{
	ym_setparam(YMVAL_AR, 0x1f, 0, 0);
	ym_setparam(YMVAL_AR, 0x1f, 0, 1);
	ym_setparam(YMVAL_AR, 0x1f, 0, 2);
	ym_setparam(YMVAL_AR, 0x1f, 0, 3);
	ym_setparam(YMVAL_RR, 0x0f, 0, 0);
	ym_setparam(YMVAL_RR, 0x0f, 0, 1);
	ym_setparam(YMVAL_RR, 0x0f, 0, 2);
	ym_setparam(YMVAL_RR, 0x0f, 0, 3);
	ym_setparam(YMVAL_KC, 0x4a, 0);
	ym_setparam(YMVAL_KON, 0);
	ym_setparam(YMVAL_KON, 0x78, 0);
	
	while(1) {}
	return 0;
}
