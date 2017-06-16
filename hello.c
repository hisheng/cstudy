#include <stdio.h>
#include <Linux/module.h>

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
		printf("age is %d \n", age);
	}

	age = max(5 , 4);
	printf("5 和 4 中  %d 大 \n", age);


	double balance[2] = {100.2,33.2};
	balance[3] = 22.2;
	for (int i = 0; i < 2; i++)

		printf("balance[%d] = %d \n",i,balance[i]);
	}

	int var1;
	printf("var1 变量的地址 ：%x\n", &var1);



	int var2 = 20; //实际变量声明
	int *ip; //指针变量申明
	
	
	ip = &var2 ; // 指针存储 var2 的地址
	printf("var2 的地址 %x \n", &var2);
	printf("指针ip 存储的 地址 %x \n",ip);
	printf("使用指针访问值 *ip %d \n", *ip);


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