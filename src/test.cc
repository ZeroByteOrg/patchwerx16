#include <stdio.h>
#include <stdint.h>


int main()
{

	char foo[4] = {'a','b','c',0};
	
	char bar[4]="xyz";
	foo[2] = 4 << -1;
	printf ("[%s]  [%d] [%d] [%d]",foo,foo[0],foo[1],foo[2]);
	while(1) {}
	return 0;
}
