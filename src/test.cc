#include <stdio.h>
#include <stdint.h>

typedef void (*foo_t)(uint8_t);

typedef struct bar_t {
	foo_t foo[2];
	foo_t foo2[2];
} bar_t;

void myfoo(uint8_t arg)
{
	printf("myfoo  - %d\n",arg);
}

void myfoo2(uint8_t arg)
{
	arg++;
	printf("myfoo2 - %d\n",arg);
}

bar_t bar;

int main()
{
	bar.foo[0]  = myfoo;
	bar.foo2[0] = myfoo2;
	bar.foo[0](12);
	bar.foo2[0](12);
	while(1) {}
	return 0;
}
