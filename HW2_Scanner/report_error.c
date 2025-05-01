#include <stdio.h>
#include <stdlib.h>

void print_error(int err_num)
{
	switch (err_num)
	{
	case 1:
		/* start with digit */
		printf("Error - Start with digit\n");
		break;
	case 2:
		printf("Error - long identifier\n");
		break;
	default:
		printf("Error - Unknown\n");
		break;
	}

}