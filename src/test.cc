#include <stdio.h>
#include <stdint.h>

typedef uint8_t (*fptr)(uint8_t);

uint8_t printd(uint8_t x)
{
	printf("%d\n",x);
	return 0;
}

uint8_t printx(uint8_t y)
{
	printf("$%2x\n",y);
	return 0;
}

uint8_t inc(uint8_t i)
{
	return i+1;
}

struct foo {
	fptr f[3];
} foo;

int main()
{
	foo.f[0] = printd;
	foo.f[1] = printx;
	foo.f[2] = inc;
	foo.f[0](0x36);
	foo.f[1](0x36);
	foo.f[0](f[2](0x36));


	while(1) {}
	return 0;
}
