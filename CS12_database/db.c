#include <stdio.h>
#include "lib/data.h"
#include "lib/lib.h" 

/* 
this is the code that is run when the database is imported
any added data is saved to the local storage when this function 
exits */
int database(){
	printf("%d\n",db_books[28].publication_date.year);
	return 12;	
}

