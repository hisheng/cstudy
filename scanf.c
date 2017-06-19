#include <stdio.h>

int main()
{
	char str[100];
	int i;

	printf("请输入值\n");

	scanf("%s %d",str,&i);

	printf( "\nYou entered: %s %d ", str, i);
   	printf("\n");

	return 0;
}