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


	int myarray[4] = {1,222,33,40};
   int *ptrArray ;
   int *p2;
   int *p3;
	ptrArray = myarray;
   p2 = myarray + 1;
   p3 = myarray + 1;
	printf("ptrArray 地址 %p \n", ptrArray);
   printf("*ptrArray = %d\n", *ptrArray);
   printf("*p2 = %d\n", *p2);
   printf("*p3 = %d\n", *p3);
   printf("myarray的占用空间 = %d\n", sizeof(myarray));
}