#include "lib/data.h"
#include "lib/lib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
this is the code that is run when the database is imported
any added data is saved to the local storage when this function
exits */
int database() {
	
	// setup a member form (type of author) 
	// the member id is a key to the new member 
	int memberid = member_add((member){

		// contact information: 
    .first_name = "Alice",
    .last_name = "Johnson",
    .email = "alice.johnson@example.com",
    .phone_number = "+1234567890",

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
			.genre ="test",
			.dod = date_now(),
			.is_alive = false,
		}

	});

	member_add((member){.first_name = "test"});
	member_add((member){.first_name = "test"});

	// copy member name
	char *name = member_name(id_to_member(memberid));


	// create a new book and link it to our new author 
	int bookid = book_add((book){

      .title = "test",
			.available = true,
			.genre = "test",
			.id_author = memberid, // use the member key from before
			.ISBN = "testtest", 
			.publication_date = date_wizard(),

  });


	// create a loan 
	loan_new(id_to_member_ptr(memberid),(loan){

			.issued = date_now(),
			.amount = 0,
			.bookid = bookid,
			
			}); 


	// create a second loan
	loan_new(id_to_member_ptr(memberid),(loan){

			.issued = date_now(),
			.amount = 0,
			.bookid = bookid,

			});
	
	for(int i = 0;i<id_to_member(memberid).loan.loan_index;i++)
		printf("%s Loans: %d\n",name, id_to_member(memberid).loan.loan_ids[i]);

	// print the total owed to the new member 
	printf("Total Owed: %d By member %s\n",total_loan(id_to_member(memberid)),name);
	


	free(name);
  return 0;
}


/*
member member_wizard(const int id);
book book_wizard(const int id);
loan loan_wizard(const int id);

void member_update(const int id);
void book_update(const int id);
void loan_update(const int id);

void member_delete(const int id);
void book_delete(const int id);
void loan_delete(const int id);
*/
