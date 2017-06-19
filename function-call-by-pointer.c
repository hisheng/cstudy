#include <stdio.h>


void swap(int *x , int *y)
{
	int tmp;
	tmp = *x; /*保存地址x的值*/
	*x = *y;
	*y = tmp;

	return ;
}

int main()
{
	/* 局部变量 定义*/
	int a = 100 ;
	int b = 200 ;

	printf("交换前 a b 的值为 %d %d\n",a,b);

	swap(&a ,&b);
	printf("交换后 a b 的值为 %d %d\n",a,b);

}