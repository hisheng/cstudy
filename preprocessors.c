#include <stdio.h>

#define  message_for(a, b)  \
    printf(#a " and " #b ": We love you!\n")


#define tokenpaster(n) printf ("token" #n " = %d", token##n)



#if !defined (MESSAGE)
   #define MESSAGE "You wish!"
#endif



#define square(x) ((x) * (x))


#define MAX(x,y) ((x) > (y) ? (x) : (y))

main()
{
	printf("File :%s\n", __FILE__ );
	printf("Date :%s\n", __DATE__ );
	printf("Time :%s\n", __TIME__ );
	printf("Line :%d\n", __LINE__ );
	printf("ANSI :%d\n", __STDC__ );
	
	message_for(Carole, Debra);


	int token34 = 40; 
   	tokenpaster(34);

   	printf("Max between 20 and 10 is %d\n", MAX(10, 20));  
}