#include <stdio.h>

//定义常量
#define USER "hisheng"



int main()
{
	//我的第一个c程序
	printf("hello world\n");

	int age;
	int *ptr; 
	age = 13;
	printf("age 的存储大小为 %lu \n",sizeof(int));
	printf("age 的值大小为 %d \n",age);

	printf("user is %s\n", USER);

	ptr = &age; //'ptr' 现在包含 'a' 的地址
	printf("*ptr 是 %d \n", ptr);

	while(age--){
		printf("age is %s\n", age);
	}

	return 0;
}