#include "lib/data.h"
#include "lib/lib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// returns &member to the id provided 
member *id_to_member_ptr(const int a){
	return  &db_members[a];
}

char *member_name(member a){
	int size = snprintf(NULL,0,"%s %s",a.first_name,a.last_name);
	char *name = (char *)malloc(size);
	sprintf(name,"%s %s",a.first_name,a.last_name);

	return name;
}

// returns member to the id provided
member id_to_member(const int a){
	return db_members[a];
}

member member_wizard(){
	char first_name[CHAR_LARGE+1];
	char last_name[CHAR_LARGE+1];
	char email[CHAR_SMALL+1];
	char phone_number[CHAR_SMALL+1];
	
	// take string input
	input(first_name,CHAR_LARGE,"First Name: ");
	input(last_name,CHAR_LARGE,"Last Name: ");
	input(email,CHAR_SMALL,"Email: ");
	input(phone_number,CHAR_SMALL,"Phone Number: ");

	return (member){};
}


/*
this is the code that is run when the database is imported
any added data is saved to the local storage when this function
exits */
int database() {
	
	// setup a member form (type of author) 
	// the member id is a key to the new member 
	int memberid = member_add((member){

		// contact information: 
    .first_name = strdup("Alice"),
    .last_name = strdup("Johnson"),
    .email = strdup("alice.johnson@example.com"),
    .phone_number = strdup("+1234567890"),

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
			.genre = strdup("test"),
			.dod = date_now(),
			.is_alive = false,
		}

	});

	// copy member name
	char *name = member_name(id_to_member(memberid));


	// create a new book and link it to our new author 
	int bookid = book_add((book){

      .title = "test",
			.available = true,
			.genre = "test",
			.id_author = memberid, // use the member key from before
			.ISBN = "testtest", 
			.publication_date = date_now(),

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
