#include "lib/data.h"
#include "lib/lib.h"
#include <stdio.h>
#include <string.h>


/*
this is the code that is run when the database is imported
any added data is saved to the local storage when this function
exits */
int database() {
	
	// setup a member (type of author) 
	member newMember = {
    .first_name = strdup("Alice"),
    .last_name = strdup("Johnson"),
    .email = strdup("alice.johnson@example.com"),
    .phone_number = strdup("+1234567890"),

    .dob = { .day = 15, .month = 5, .year = 1990 },
    .time_created = date_now(),
    .type = AUTHOR,
		.o.author.genre = strdup("test"),
		.o.author.dod = date_now(),
		.o.author.is_alive = false,
	};

	// create a member
	int member_id = member_add(newMember);

	// create a new book and link it to our new author 
	int bookid = book_add((book){
      .title = "test",
			.available = true,
			.genre = "test",
			.id_author = member_id,
			.ISBN = "testtest", 
			.publication_date = date_now(),
  });

	
	// create a loan 
	loan_new(&newMember,(loan){
			.issued = date_now(),
			.amount = 1234,
			}); 

	// create a second loan
	loan_new(&newMember,(loan){
			.issued = date_now(),
			.amount = 1525,
			}); 
	
	printf("Loans of member:\n");
	
	for(int i = 0;i<newMember.loan.loan_index;i++)
		printf("%d\n",newMember.loan.loan_ids[i]);

	printf("Total Owed: %d By member %s %s\n",total_loan(newMember), newMember.first_name, newMember.last_name);

  return 0;
}
