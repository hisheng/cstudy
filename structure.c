#include <stdio.h>
#include <string.h>

struct Books
{
	char title[50];
	char author[50];
	char subject[100];
	int book_id;
};

int main()
{
	struct Books Book1;
	struct Books Book2;

	/* Book1 */
	strcpy(Book1.title , "c programming");
	Book1.author = {'h','i','s','h','e','n','g'};
	strcpy(Book1.subject = 'cc');
	Book1.book_id = 1134;

	/* Book2*/
	strcpy(Book1.title , "PHP programming");
	strcpy(Book1.author = 'cc'); 
	strcpy(Book1.subject = 'php');
	Book1.book_id = 113444;

	 /* 输出 Book1 信息 */
   printf( "Book 1 title : %s\n", Book1.title);
   printf( "Book 1 author : %s\n", Book1.author);
   printf( "Book 1 subject : %s\n", Book1.subject);
   printf( "Book 1 book_id : %d\n", Book1.book_id);
 
   /* 输出 Book2 信息 */
   printf( "Book 2 title : %s\n", Book2.title);
   printf( "Book 2 author : %s\n", Book2.author);
   printf( "Book 2 subject : %s\n", Book2.subject);
   printf( "Book 2 book_id : %d\n", Book2.book_id);
 
   return 0;

}