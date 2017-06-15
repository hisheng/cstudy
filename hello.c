#include <stdio.h>

//定义常量
#define USER "hisheng"


//声明比较大小函数
int max(int n1 , int n2);


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

	age = max(5 , 4);
	printf("5 和 4 中  %d 大 \n", age);

	return 0;
}

int max(int n1 , int n2)
{
	int result ;

	if (n1 > n2)
	 	result = n1 ;
	else
		result = n2 ;

	return result ;
}