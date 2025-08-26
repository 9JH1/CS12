#include "lib/data.h"
#include "lib/lib.h"
#include <stdio.h>

int database() {
	
	// create a new member and store its unique id in the memberid variable
	int memberid = member_add((member){

		// contact information: 
    .first_name = s("Alice"),
    .last_name = s("Johnson"),
    .email = s("alice.johnson@example.com"),
    .phone_number = s("+1234567890"),

		// time settings: 
    .dob = {
			.day = 15,
			.month = 5,
			.year = 1990 
		},
    .time_created = date_now(),
    
		// member type settings: 
		.type = AUTHOR,
		.o.author = {
			.genre = s("test"),
			.dod = date_now(),
			.is_alive = false,
		}
	});


	// copy member name
	char *name = member_name(id_to_member(memberid));

	// run the date wizard: 
	const date newdate = date_wizard();
	if(newdate.year == -1){
		printf("Date wizard exited\n");
		return -1;
	}


	// create a new book and link it to our new author 
	int bookid = book_add((book){
      .title = "test",
			.available = true,
			.genre = "test",
			.id_author = memberid, // use the member key from before
			.ISBN = "testtest", 
			.publication_date = newdate,
  });


	// create a loan 
	loan_new(id_to_member_ptr(memberid),(loan){
			.issued = date_now(),
			.amount = 12,
			.bookid = bookid,
			});


	// create a second loan
	loan_new(id_to_member_ptr(memberid),(loan){
			.issued = date_now(),
			.amount = 12,
			.bookid = bookid,
			});
	
	printf("User %s has these loans:\n",name);
	for(int i = 0;i<id_to_member(memberid).loan.loan_index;i++)
		printf("#%d (loanid %d): Bookid %d, fee $%d\n",i,
				id_to_member(memberid).loan.loan_ids[i],
				db_loans[id_to_member(memberid).loan.loan_ids[i]].bookid,
				db_loans[id_to_member(memberid).loan.loan_ids[i]].amount);


	// print the total owed to the new member 
	printf("Total Owed: %d By member %s\n",total_loan(id_to_member(memberid)),name);
	

	// free the alocated memory for the name
	free(name);
  return 0;
}
