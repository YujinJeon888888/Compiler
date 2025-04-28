#include <stdio.h>
#include <stdlib.h>

/**
 * @brief print_error: 에러메시지 출력
 * @param err_num: 에러번호
 */
void print_error(int err_num)
{
	//에러넘버에 따라 달리 출력
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