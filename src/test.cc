#include <stdio.h>
#include <stdint.h>

typedef uint8_t (*handler)(uint8_t);

typedef struct foo_t {
	const char* msg;
	handler doit;
} foo_t;

uint8_t answer(uint8_t f)
{
	printf("the answer is %d\n",f);
	return 0;
}

uint8_t question(uint8_t f)
{
	printf("what is 6 * %d?\n",f);
	return 0;
}

foo_t foo;

int main()
{
	foo.msg = "hello world.";
	foo.doit = question;
	foo.doit(7);
	foo.doit = answer;
	foo.doit(42);
	while(1) {}
	return 0;
}
