#include "ym2151.h"
#include <stdio.h>

int main()
	ym_patchregs p = {};
	p[12]=4;
	printf("%d %d\n",p[4],p[12]);
	return 0;
}
