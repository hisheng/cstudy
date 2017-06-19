#include <stdio.h> 
//定义常量
#define USER "hisheng"


typedef int (*fun_ptr) (int ,int);

int max(int x , int y)
{
	return x > y ? x : y ;
}

int main (void)
{
	int (*p)(int , int) = max ;
	int a , b , c , d ;

	printf("请输入三个数字\n");
	scanf("%d %d %d ",&a ,&b ,&c);

	d = p(p(a,b),c);
	printf("最大的数组是 ： %d \n", d);
}