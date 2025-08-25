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

int eatoi(const char *a){
	int out = atoi(a);
	if(out == 0 && strcmp(a,"0")!=0) return -1;
	return out;
}

date date_wizard(){
	char year[CHAR_SMALL+1];
	char month[CHAR_SMALL+1];
	char day[CHAR_SMALL+1];
	char hour[CHAR_SMALL+1];
	char minute[CHAR_SMALL+1];
	char second[CHAR_SMALL+1];
	date newdate = {-1};
	
	// tip
	printf("Ensure any values of 0 are formatted like '0' not '00' or otherwise\n");

	// take inputs (with error checking)
	input(year,CHAR_SMALL,"Enter Year: ");
	int iyear = eatoi(year);
	if(iyear == -1){
		printf("year \"%s\" is invalid\n",year);
		return newdate;
	}

	input(month,CHAR_SMALL,"Enter Month (1-12): ");
	int imonth = eatoi(month);
	if (imonth == -1 || imonth < 0 || imonth > 12){
		printf("month \"%s\" is invalid\n",month);
		return newdate;	
	}

	input(day, CHAR_SMALL,"Enter Day (0-31): ");
	int iday = eatoi(day);
	if( iday == -1 ||  iday < 0 || iday > 31){
		printf("day \"%s\" is invalid\n",day);
		return newdate;
	} 
	
	input(hour, CHAR_SMALL,"Enter Hour (0-23): ");
	int ihour = eatoi(hour);
	if (ihour == -1 ||  ihour < 0 || ihour > 23){
		printf("hour \"%s\" is invalid\n",hour);
		return newdate;
	}

	input(minute,CHAR_SMALL, "Enter Minute (0-59): ");
	int iminute = eatoi(minute);
	if (iminute == -1 || iminute < 0 || iminute > 60){
		printf("minute \"%s\" is invalid\n",minute);
		return newdate;
	}

	input(second, CHAR_SMALL, "Enter Second (0-59): ");
	int isecond = eatoi(second);
	if (isecond == -1 || isecond < 0 || isecond > 60){
		printf("second \"%s\" is invalid\n",second);
		return newdate;
	}
	
	// create date object
	newdate = (date){
		.year = iyear,
		.month = imonth,
		.day = iday,
		.hour = ihour,
		.minute = iminute,
		.second = isecond,
	};

	// return the created date
	return newdate;
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

	char type[CHAR_SMALL+1];
	input(type,CHAR_SMALL,"Enter Account type (MEMBER/author/staff): ");
	if(strcmp(type,"author")==0){
	}
	
	return (member){
		.first_name = first_name,
		.last_name = last_name,
		.email = email,
		.phone_number = phone_number,
	};
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
	

	// free the alocated memory for the name
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
