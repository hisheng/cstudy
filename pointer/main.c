#include <stdio.h>

void change(int *left , int *right)
{
	int tmp = *left;
	*left = *right;
	*right = tmp; 
}

int main()
{
	int a = 3;
	int b = 5;
	change(&a,&b);
	printf("num a = %d \n num b = %d ",a ,b);
	return 0;
}