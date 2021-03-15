#include <stdio.h>
#include <stdlib.h>

void __cdecl main()
{
	printf("Program built on " __DATE__" at "__TIME__"\n");
	printf("File '%s'\n",  __FILE__);
	printf("Line %d, ",__LINE__);
	printf("function '"__FUNCTION__"'\n");

	printf("\n\t\t\t'HELLO WORLD !!!'");
	
	printf("\n\nPress 'Enter' to exit..."); getchar();
	return;
}
