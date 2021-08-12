#include <stdio.h>
#include <stdint.h>
#include "ym2151.c"

uint8_t i;

void playtone()
{
	ym_set_param(YMVAL_L, 1, 0);
	ym_set_param(YMVAL_R, 1, 0);
	ym_set_param(YMVAL_CONECT, 7, 0);
	ym_set_param(YMVAL_FB, 0, 0);
	ym_set_param(YMVAL_TL, 0x06, 0, 0);
	ym_set_param(YMVAL_TL, 0x7f, 0, 1);
	ym_set_param(YMVAL_TL, 0x7f, 0, 2);
	ym_set_param(YMVAL_TL, 0x7f, 0, 3);
	ym_set_param(YMVAL_AR, 0x0f, 0, 0);
	ym_set_param(YMVAL_AR, 0x1f, 0, 0);
	ym_set_param(YMVAL_AR, 0x1f, 0, 1);
	ym_set_param(YMVAL_AR, 0x1f, 0, 2);
	ym_set_param(YMVAL_AR, 0x1f, 0, 3);
	ym_set_param(YMVAL_RR, 0x0f, 0, 0);
	ym_set_param(YMVAL_RR, 0x0f, 0, 1);
	ym_set_param(YMVAL_RR, 0x0f, 0, 2);
	ym_set_param(YMVAL_RR, 0x0f, 0, 3);
	ym_set_param(YMVAL_KC, 0x4a, 0);
	ym_set_param(YMVAL_KON, 0);
	ym_set_param(YMVAL_KON, 0x78);
}

void showmap()
{
	for (i = YMVAL_TEST ; i<YMVAL_count ; i++)
		printf("%02d map: %02x %02x %02x\n", i, YMREG(i), VALMASK(i), VALSHIFT(i));
}

int main()
{
	printf("initializing... ");
	ym_init();
	printf("done\n");
	playtone();
	while(1) {}
	return 0;
}
