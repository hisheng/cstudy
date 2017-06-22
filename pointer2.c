#include <stdio.h>


int main()
{
	int var1;
	char var2[12];

	printf("var1 变量的地址： %x\n", &var1);
	printf("var1 变量的地址： %d\n", &var1);
   	printf("var2 变量的地址： %x\n", &var2);
   	printf("var1 变量的地址： %d\n", &var2);

   	int *ptr;
   	int val = 1;

   	ptr = &val;

   	printf("ptr 变量的地址： %d\n", ptr);
   	printf("ptr 变量的地址  的值(*ptr)： %d\n", *ptr);
}