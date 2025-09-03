#include "lib/data.h"
#include "lib/lib.h"
#include <stdio.h>
#include <string.h>

char *set_string(char *dest, const char *src, size_t size) {
    if (dest == NULL || src == NULL || size == 0) {
        return dest; // Return unchanged dest for safety
    }
    strncpy(dest, src, size - 1);
    dest[size - 1] = '\0'; // Ensure null-termination
    return dest;
}


/*
this is the code that is run when the database is imported
any added data is saved to the local storage when this function
exits */
int database() {
	
	// setup a member form (type of author) 
	// the member id is a key to the new member 
	member m = {
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
	};
	set_string(m.first_name,"John",CHAR_SMALL);
	set_string(m.last_name,"Doe",CHAR_SMALL);

	const int memberid = member_add(m); 

	member_add((member){0});
	member_add((member){0});

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
#include "lib/data.h"
#include "lib/lib.h"

// data setup
int database() {
  // =====================================================
  // CREATE AUTHORS
  // =====================================================
  const int tolkien = member_add((member){
      .first_name = "J.R.R.",
      .last_name = "Tolkien",
      .type = AUTHOR,
      .o.author.genre = "Fantasy",
  });

  const int rowling = member_add((member){
      .first_name = "Joanne",
      .last_name = "Rowling",
      .type = AUTHOR,
      .o.author.genre = "Fantasy",
  });

  const int king = member_add((member){
      .first_name = "Stephen",
      .last_name = "King",
      .type = AUTHOR,
      .o.author.genre = "Horror",
  });

  const int austen = member_add((member){
      .first_name = "Jane",
      .last_name = "Austen",
      .type = AUTHOR,
      .o.author.genre = "Romance",
  });

  const int orwell = member_add((member){
      .first_name = "George",
      .last_name = "Orwell",
      .type = AUTHOR,
      .o.author.genre = "Dystopian",
  });

  const int hemingway = member_add((member){
      .first_name = "Ernest",
      .last_name = "Hemingway",
      .type = AUTHOR,
      .o.author.genre = "Literary Fiction",
  });

  const int christie = member_add((member){
      .first_name = "Agatha",
      .last_name = "Christie",
      .type = AUTHOR,
      .o.author.genre = "Mystery",
  });

  const int asimov = member_add((member){
      .first_name = "Isaac",
      .last_name = "Asimov",
      .type = AUTHOR,
      .o.author.genre = "Science Fiction",
  });

  const int dickens = member_add((member){
      .first_name = "Charles",
      .last_name = "Dickens",
      .type = AUTHOR,
      .o.author.genre = "Historical Fiction",
  });

  const int adams = member_add((member){
      .first_name = "Douglas",
      .last_name = "Adams",
      .type = AUTHOR,
      .o.author.genre = "Science Fiction",
  });

  // =====================================================
  // CREATE BOOKS
  // =====================================================
  const int the_hobbit = book_add((book){
      .title = "The Hobbit",
      .id_author = tolkien,
      .publication_date =
          (date){
              .year = 1937,
              .month = 9,
              .day = 21,
              .hour = 0,
              .minute = 0,
              .second = 0,
          },
      .ISBN = "978-0261102217",
      .genre = "Fantasy",
      .available = 10,
      .count = 10,
  });

  const int harry_potter = book_add((book){
      .title = "Harry Potter and the Philosopher's Stone",
      .id_author = rowling,
      .publication_date =
          (date){
              .year = 1997,
              .month = 6,
              .day = 26,
              .hour = 0,
              .minute = 0,
              .second = 0,
          },
      .ISBN = "978-0747532699",
      .genre = "Fantasy",
      .available = 10,
      .count = 10,
  });

  const int the_shining = book_add((book){
      .title = "The Shining",
      .id_author = king,
      .publication_date =
          (date){
              .year = 1977,
              .month = 1,
              .day = 28,
              .hour = 0,
              .minute = 0,
              .second = 0,
          },
      .ISBN = "978-0385121675",
      .genre = "Horror",
      .available = 10,
      .count = 10,
  });

  const int pride_and_prejudice = book_add((book){
      .title = "Pride and Prejudice",
      .id_author = austen,
      .publication_date =
          (date){
              .year = 1813,
              .month = 1,
              .day = 28,
              .hour = 0,
              .minute = 0,
              .second = 0,
          },
      .ISBN = "978-0141439518",
      .genre = "Romance",
      .available = 10,
      .count = 10,
  });

  const int nineteen_eighty_four = book_add((book){
      .title = "1984",
      .id_author = orwell,
      .publication_date =
          (date){
              .year = 1949,
              .month = 6,
              .day = 8,
              .hour = 0,
              .minute = 0,
              .second = 0,
          },
      .ISBN = "978-0451524935",
      .genre = "Dystopian",
      .available = 10,
      .count = 10,
  });

  const int old_man_and_sea = book_add((book){
      .title = "The Old Man and the Sea",
      .id_author = hemingway,
      .publication_date =
          (date){
              .year = 1952,
              .month = 9,
              .day = 1,
              .hour = 0,
              .minute = 0,
              .second = 0,
          },
      .ISBN = "978-0684830490",
      .genre = "Literary Fiction",
      .available = 10,
      .count = 10,
  });

  const int poirot_murder = book_add((book){
      .title = "Murder on the Orient Express",
      .id_author = christie,
      .publication_date =
          (date){
              .year = 1934,
              .month = 1,
              .day = 1,
              .hour = 0,
              .minute = 0,
              .second = 0,
          },
      .ISBN = "978-0062693662",
      .genre = "Mystery",
      .available = 10,
      .count = 10,
  });

  const int foundation = book_add((book){
      .title = "Foundation",
      .id_author = asimov,
      .publication_date =
          (date){
              .year = 1951,
              .month = 5,
              .day = 1,
              .hour = 0,
              .minute = 0,
              .second = 0,
          },
      .ISBN = "978-0553293357",
      .genre = "Science Fiction",
      .available = 10,
      .count = 10,
  });

  const int great_expectations = book_add((book){
      .title = "Great Expectations",
      .id_author = dickens,
      .publication_date =
          (date){
              .year = 1861,
              .month = 8,
              .day = 1,
              .hour = 0,
              .minute = 0,
              .second = 0,
          },
      .ISBN = "978-0141439563",
      .genre = "Historical Fiction",
      .available = 10,
      .count = 10,
  });

  const int hitchhiker = book_add((book){
      .title = "The Hitchhiker's Guide to the Galaxy",
      .id_author = adams,
      .publication_date =
          (date){
              .year = 1979,
              .month = 10,
              .day = 12,
              .hour = 0,
              .minute = 0,
              .second = 0,
          },
      .ISBN = "978-0345391803",
      .genre = "Science Fiction",
      .available = 10,
      .count = 10,
  });


  // =====================================================
  // CREATE USERS
  // =====================================================
  const int alice_smith =
      member_add((member){.first_name = "Alice",
                          .last_name = "Smith",
                          .dob =
                              (date){
                                  .year = 1990,
                                  .month = 3,
                                  .day = 15,
                                  .hour = 0,
                                  .minute = 0,
                                  .second = 0,
                              },
                          .type = MEMBER,
                          .email = "alice.smith@email.com",
                          .phone_number = "555-0101",
                          .time_created = date_now()});

  const int bob_johnson =
      member_add((member){.first_name = "Bob",
                          .last_name = "Johnson",
                          .dob =
                              (date){
                                  .year = 1985,
                                  .month = 7,
                                  .day = 22,
                                  .hour = 0,
                                  .minute = 0,
                                  .second = 0,
                              },
                          .type = MEMBER,
                          .email = "bob.johnson@email.com",
                          .phone_number = "555-0102",
                          .time_created = date_now()});

  const int clara_wong = member_add((member){.first_name = "Clara",
                                             .last_name = "Wong",
                                             .dob =
                                                 (date){
                                                     .year = 1995,
                                                     .month = 11,
                                                     .day = 8,
                                                     .hour = 0,
                                                     .minute = 0,
                                                     .second = 0,
                                                 },
                                             .type = MEMBER,
                                             .email = "clara.wong@email.com",
                                             .phone_number = "555-0103",
                                             .time_created = date_now()});

  const int david_martinez =
      member_add((member){.first_name = "David",
                          .last_name = "Martinez",
                          .dob =
                              (date){
                                  .year = 1988,
                                  .month = 4,
                                  .day = 30,
                                  .hour = 0,
                                  .minute = 0,
                                  .second = 0,
                              },
                          .type = MEMBER,
                          .email = "david.martinez@email.com",
                          .phone_number = "555-0104",
                          .time_created = date_now()});

  const int emma_brown = member_add((member){.first_name = "Emma",
                                             .last_name = "Brown",
                                             .dob =
                                                 (date){
                                                     .year = 1992,
                                                     .month = 9,
                                                     .day = 12,
                                                     .hour = 0,
                                                     .minute = 0,
                                                     .second = 0,
                                                 },
                                             .type = MEMBER,
                                             .email = "emma.brown@email.com",
                                             .phone_number = "555-0105",
                                             .time_created = date_now()});

  const int frank_lee = member_add((member){.first_name = "Frank",
                                            .last_name = "Lee",
                                            .dob =
                                                (date){
                                                    .year = 1983,
                                                    .month = 2,
                                                    .day = 19,
                                                    .hour = 0,
                                                    .minute = 0,
                                                    .second = 0,
                                                },
                                            .type = MEMBER,
                                            .email = "frank.lee@email.com",
                                            .phone_number = "555-0106",
                                            .time_created = date_now()});

  const int grace_kim = member_add((member){.first_name = "Grace",
                                            .last_name = "Kim",
                                            .dob =
                                                (date){
                                                    .year = 1998,
                                                    .month = 12,
                                                    .day = 5,
                                                    .hour = 0,
                                                    .minute = 0,
                                                    .second = 0,
                                                },
                                            .type = MEMBER,
                                            .email = "grace.kim@email.com",
                                            .phone_number = "555-0107",
                                            .time_created = date_now()});

  const int henry_davis =
      member_add((member){.first_name = "Henry",
                          .last_name = "Davis",
                          .dob =
                              (date){
                                  .year = 1980,
                                  .month = 6,
                                  .day = 25,
                                  .hour = 0,
                                  .minute = 0,
                                  .second = 0,
                              },
                          .type = MEMBER,
                          .email = "henry.davis@email.com",
                          .phone_number = "555-0108",
                          .time_created = date_now()});

  const int isabella_nguyen =
      member_add((member){.first_name = "Isabella",
                          .last_name = "Nguyen",
                          .dob =
                              (date){
                                  .year = 1993,
                                  .month = 8,
                                  .day = 17,
                                  .hour = 0,
                                  .minute = 0,
                                  .second = 0,
                              },
                          .type = MEMBER,
                          .email = "isabella.nguyen@email.com",
                          .phone_number = "555-0109",
                          .time_created = date_now()});

  const int james_park = member_add((member){.first_name = "James",
                                             .last_name = "Park",
                                             .dob =
                                                 (date){
                                                     .year = 1987,
                                                     .month = 1,
                                                     .day = 10,
                                                     .hour = 0,
                                                     .minute = 0,
                                                     .second = 0,
                                                 },
                                             .type = MEMBER,
                                             .email = "james.park@email.com",
                                             .phone_number = "555-0110",
                                             .time_created = date_now()});


  // ======================================================
  // CREATE STAFF
  // ======================================================
  const int member_1 = member_add((member){.type = STAFF,
                                           .first_name = "Tim",
                                           .last_name = "Buck",
                                           .dob =
                                               (date){
                                                   .year = 1987,
                                                   .month = 1,
                                                   .day = 10,
                                                   .hour = 0,
                                                   .minute = 0,
                                                   .second = 0,
                                               },
                                           .email = "Tim.Buck2@email.com",
                                           .phone_number = "555-01112",
                                           .time_created = date_now(),
                                           .o.staff = {
                                               .member_code = db_members_index,
                                               .member_id = db_members_index,
                                               .is_hired = true,
                                           }});

  const int member_2 = member_add((member){.type = STAFF,
                                           .first_name = "Emma",
                                           .last_name = "Wilson",
                                           .dob =
                                               (date){
                                                   .year = 1990,
                                                   .month = 5,
                                                   .day = 22,
                                                   .hour = 0,
                                                   .minute = 0,
                                                   .second = 0,
                                               },
                                           .email = "emma.wilson@email.com",
                                           .phone_number = "555-0111",
                                           .time_created = date_now(),
                                           .o.staff = {
                                               .member_code = db_members_index,
                                               .member_id = db_members_index,
                                               .is_hired = true,
                                           }});

  const int member_3 = member_add((member){.type = STAFF,
                                           .first_name = "Liam",
                                           .last_name = "Chen",
                                           .dob =
                                               (date){
                                                   .year = 1985,
                                                   .month = 8,
                                                   .day = 15,
                                                   .hour = 0,
                                                   .minute = 0,
                                                   .second = 0,
                                               },
                                           .email = "liam.chen@email.com",
                                           .phone_number = "555-0112",
                                           .time_created = date_now(),
                                           .o.staff = {
                                               .member_code = db_members_index,
                                               .member_id = db_members_index,
                                               .is_hired = true,
                                           }});

  const int member_4 =
      member_add((member){.type = STAFF,
                          .first_name = "Sophia",
                          .last_name = "Martinez",
                          .dob =
                              (date){
                                  .year = 1992,
                                  .month = 3,
                                  .day = 7,
                                  .hour = 0,
                                  .minute = 0,
                                  .second = 0,
                              },
                          .email = "sophia.martinez@email.com",
                          .phone_number = "555-0113",
                          .time_created = date_now(),
                          .o.staff = {
                              .member_code = db_members_index,
                              .member_id = db_members_index,
                              .is_hired = true,
                          }});

  const int member_5 = member_add((member){.type = STAFF,
                                           .first_name = "Noah",
                                           .last_name = "Davis",
                                           .dob =
                                               (date){
                                                   .year = 1988,
                                                   .month = 11,
                                                   .day = 30,
                                                   .hour = 0,
                                                   .minute = 0,
                                                   .second = 0,
                                               },
                                           .email = "noah.davis@email.com",
                                           .phone_number = "555-0114",
                                           .time_created = date_now(),
                                           .o.staff = {
                                               .member_code = db_members_index,
                                               .member_id = db_members_index,
                                               .is_hired = true,
                                           }});

  const int member_6 = member_add((member){.type = STAFF,
                                           .first_name = "Olivia",
                                           .last_name = "Brown",
                                           .dob =
                                               (date){
                                                   .year = 1995,
                                                   .month = 7,
                                                   .day = 19,
                                                   .hour = 0,
                                                   .minute = 0,
                                                   .second = 0,
                                               },
                                           .email = "olivia.brown@email.com",
                                           .phone_number = "555-0115",
                                           .time_created = date_now(),
                                           .o.staff = {
                                               .member_code = db_members_index,
                                               .member_id = db_members_index,
                                               .is_hired = true,
                                           }});

  const int member_7 = member_add((member){.type = STAFF,
                                           .first_name = "Ethan",
                                           .last_name = "Taylor",
                                           .dob =
                                               (date){
                                                   .year = 1983,
                                                   .month = 2,
                                                   .day = 25,
                                                   .hour = 0,
                                                   .minute = 0,
                                                   .second = 0,
                                               },
                                           .email = "ethan.taylor@email.com",
                                           .phone_number = "555-0116",
                                           .time_created = date_now(),
                                           .o.staff = {
                                               .member_code = db_members_index,
                                               .member_id = db_members_index,
                                               .is_hired = true,
                                           }});

  const int member_8 = member_add((member){.type = STAFF,
                                           .first_name = "Ava",
                                           .last_name = "Lee",
                                           .dob =
                                               (date){
                                                   .year = 1991,
                                                   .month = 9,
                                                   .day = 12,
                                                   .hour = 0,
                                                   .minute = 0,
                                                   .second = 0,
                                               },
                                           .email = "ava.lee@email.com",
                                           .phone_number = "555-0117",
                                           .time_created = date_now(),
                                           .o.staff = {
                                               .member_code = db_members_index,
                                               .member_id = db_members_index,
                                               .is_hired = true,
                                           }});

  const int member_9 = member_add((member){.type = STAFF,
                                           .first_name = "Lucas",
                                           .last_name = "Garcia",
                                           .dob =
                                               (date){
                                                   .year = 1989,
                                                   .month = 4,
                                                   .day = 5,
                                                   .hour = 0,
                                                   .minute = 0,
                                                   .second = 0,
                                               },
                                           .email = "lucas.garcia@email.com",
                                           .phone_number = "555-0118",
                                           .time_created = date_now(),
                                           .o.staff = {
                                               .member_code = db_members_index,
                                               .member_id = db_members_index,
                                               .is_hired = true,
                                           }});

  const int member_10 = member_add((member){.type = STAFF,
                                            .first_name = "Mia",
                                            .last_name = "Johnson",
                                            .dob =
                                                (date){
                                                    .year = 1994,
                                                    .month = 6,
                                                    .day = 28,
                                                    .hour = 0,
                                                    .minute = 0,
                                                    .second = 0,
                                                },
                                            .email = "mia.johnson@email.com",
                                            .phone_number = "555-0119",
                                            .time_created = date_now(),
                                            .o.staff = {
                                                .member_code = db_members_index,
                                                .member_id = db_members_index,
                                                .is_hired = true,
                                            }});

	// =============================================
	// ATTACH LOANS
	// =============================================
	loan_new(id_to_member_ptr(member_1),(loan){
			.bookid = old_man_and_sea,
			.issued = date_now(),
			.return_date = (date){
				.year = 3000,
				.month = 1,
				.day = 1,
				
				.hour = 1,
				.minute = 1,
				.second = 1,
			},
			.active = true,
			.note = "This user is an owner and can have a book for a long time",
			.amount = 0, // owes nothing
			});

	loan_new(id_to_member_ptr(member_2),(loan){
			.bookid = the_hobbit,
			.issued = date_now(),
			.return_date = (date){
				.year = 2026,
				.month = 7,
				.day = 26,

				.hour = 5,
				.minute = 3,
				.second = 56,
			},

				.active = true,
				.amount = 3, // three (credits)
			});
	
	// actual database
	printf("Here is all the data held:\n");
	for(int i = 0;i<db_members_index; i++){
		member cur_member = id_to_member(i);
		char *name = member_name(cur_member);
		printf("%2i. %s\n",i,name);

		printf("Member has these loans:");
		for(int ii = 0; ii<cur_member.loan.loan_index;ii++){
			loan cur_loan = db_loans[cur_member.loan.loan_ids[ii]];
			book cur_book = db_books[cur_loan.bookid];
			member cur_author = id_to_member(cur_book.id_author);
			char *author_name = member_name(cur_author);

			printf("%2i - Loan for book %s\n",ii,cur_book.title);
			printf("%2i - This loan costs $%d\n",ii,cur_loan.amount);
			printf("%2i - This loan was issued at %d/%d/%d, %d:%d:%d\n",ii,
					cur_loan.issued.year,
					cur_loan.issued.month,
					cur_loan.issued.day,
					cur_loan.issued.hour,
					cur_loan.issued.minute,
					cur_loan.issued.second);

			printf("%2i - This loan is due at %d/%d/%d, %d:%d:%d\n",ii,
					cur_loan.return_date.year,
					cur_loan.return_date.month,
					cur_loan.return_date.day,
					cur_loan.return_date.hour,
					cur_loan.return_date.minute,
					cur_loan.return_date.second);

			printf("%2i - This book in particular was published %d/%d/%d, %d:%d:%d\n",ii,
					cur_book.publication_date.year, 
					cur_book.publication_date.month, 
					cur_book.publication_date.day, 
					cur_book.publication_date.hour, 
					cur_book.publication_date.minute, 
					cur_book.publication_date.second);

			printf("%2i - This book was written by %s\n",ii,author_name);
		}

		free(name);
	}

		// show all loans show total owing 
		// show all books and their genres and the amount available 
		// show all members and how many loans they have and how much they owe


	return 12; }
#include "lib.h"
#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// local database 
loan *db_loans = NULL;
int db_loans_index = 0;
int db_loans_capacity = 0;

member *db_members = NULL;
int db_members_index = 0 ;
int db_members_capacity = 0;

book *db_books;
int db_books_index;
int db_books_capacity;


void input(char *buffer, int size, char *prompt) {
  printf("%s", prompt);
  if (fgets(buffer, size, stdin) != NULL) {
    // Remove newline character if present
    int len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
      buffer[len - 1] = '\0';
  }
}

char random_char(void) {
  // Seed the random number generator (call once in program)
  static int seeded = 0;
  if (!seeded) {
    srand(time(NULL));
    seeded = 1;
  }

  // Generate random printable ASCII character (from 33 to 126)
  return (char)(rand() % (126 - 33 + 1) + 33);
}

void write_string(FILE *fp, const char *str) {
  int len = str ? (int)strlen(str) : 0;
  fwrite(&len, sizeof(int), 1, fp);
  if (len > 0) fwrite(str, sizeof(char), len, fp);
}

char *read_string(FILE *fp) {
  int len;
  fread(&len, sizeof(int), 1, fp);
  if (len == 0)
    return NULL;
  char *str = malloc(len + 1);
  fread(str, sizeof(char), len, fp);
  str[len] = '\0';
  return str;
}

void write_loans(FILE *fp, loan *arr, int count) {
  fwrite(&count, sizeof(int), 1, fp);
  for (int i = 0; i < count; i++) {
    fwrite(&arr[i].amount, sizeof(int), 1, fp);
    fwrite(&arr[i].issued, sizeof(date), 1, fp);
    fwrite(&arr[i].returned, sizeof(date), 1, fp);
		fwrite(&arr[i].return_date, sizeof(date),1,fp);
		fwrite(&arr[i].bookid,sizeof(int),1,fp);
    fwrite(&arr[i].active, sizeof(bool), 1, fp);
    write_string(fp, arr[i].note);
    fwrite(&arr[i].is.payed, sizeof(bool), 1, fp); 
  }
}

loan *read_loans(FILE *fp, int *out_count) {
  fread(out_count, sizeof(int), 1, fp);
  loan *arr = malloc(*out_count * sizeof(loan));
  for (int i = 0; i < *out_count; i++) {
    fread(&arr[i].amount, sizeof(int), 1, fp);
    fread(&arr[i].issued, sizeof(date), 1, fp);
    fread(&arr[i].returned, sizeof(date), 1, fp);
		fread(&arr[i].return_date,sizeof(date),1,fp);
		fread(&arr[i].bookid,sizeof(int),1,fp);
    fread(&arr[i].active, sizeof(bool), 1, fp);
    fread(&arr[i].note, CHAR_LARGE,1,fp);
    fread(&arr[i].is.payed, sizeof(bool), 1, fp);
  }
  return arr;
}

void write_books(FILE *fp, book *arr, int count) {
  fwrite(&count, sizeof(int), 1, fp);
  for (int i = 0; i < count; i++) {
    fwrite(arr[i].title, CHAR_LARGE, 1, fp);
    fwrite(&arr[i].id_author, sizeof(int), 1, fp);
    fwrite(&arr[i].publication_date, sizeof(date), 1, fp);
    fwrite(arr[i].genre, CHAR_SMALL, 1, fp);
    fwrite(arr[i].ISBN, 15, 1, fp);
    fwrite(&arr[i].available, sizeof(bool), 1, fp);
  }
}

book *read_books(FILE *fp, int *out_count) {
  fread(out_count, sizeof(int), 1, fp);
  book *arr = malloc(*out_count * sizeof(book));
  for (int i = 0; i < *out_count; i++) {
    fread(arr[i].title, CHAR_LARGE, 1, fp);
    fread(&arr[i].id_author, sizeof(int), 1, fp);
    fread(&arr[i].publication_date, sizeof(date), 1, fp);
    fread(arr[i].genre, CHAR_SMALL, 1, fp);
    fread(arr[i].ISBN, 15, 1, fp);
    fread(&arr[i].available, sizeof(bool), 1, fp);
  }
  return arr;
}

void write_members(FILE *a, member *arr, int size) {
  if (!a || !arr || size < 0) return;
	for(int i = 0;i < size; ++i) if(db_members[i].account_to_delete == true) size--;
  fwrite(&size, sizeof(int), 1, a);

  for (int i = 0; i < size; ++i) {
    const member *m = &arr[i];
		if(m->account_to_delete) continue;

		fwrite(m->first_name,CHAR_SMALL,1,a);
		fwrite(m->last_name, CHAR_SMALL,1,a);
		fwrite(m->email,CHAR_SMALL,1,a);
		fwrite(m->phone_number,CHAR_SMALL,1,a);

    fwrite(&m->dob, sizeof(date), 1, a);
    fwrite(&m->time_created, sizeof(date), 1, a);
    fwrite(&m->type, sizeof(memberType), 1, a);
		fwrite(&m->account_available,sizeof(bool), 1, a);

    // Write loan data
    fwrite(&m->loan.loan_flagged, sizeof(bool), 1, a);
    fwrite(&m->loan.loan_index, sizeof(int), 1, a);
    fwrite(&m->loan.loan_capacity, sizeof(int), 1, a);

    if (m->loan.loan_capacity > 0 && m->loan.loan_ids != NULL) {
      fwrite(m->loan.loan_ids, sizeof(int), m->loan.loan_capacity, a);
    }

    // Union data based on member type
    if (m->type == STAFF) {
      fwrite(&m->o.staff.is_hired, sizeof(bool), 1, a);
      fwrite(&m->o.staff.member_id, sizeof(int), 1, a);
      fwrite(&m->o.staff.member_code, sizeof(int), 1, a);
    } else if (m->type == AUTHOR) {
      fwrite(&m->o.author.genre,CHAR_SMALL,1,a);
      fwrite(&m->o.author.dod, sizeof(date), 1, a);
      fwrite(&m->o.author.is_alive, sizeof(bool), 1, a);
    }

  }
}

member *read_members(FILE *a, int *out_size) {
    FILE *fp = (FILE *)a;
    int size = 0;
    if (fread(&size, sizeof(int), 1, fp) != 1 || size < 0) {
        printf("Error: Invalid size in members file\n");
        *out_size = 0;
        return malloc(sizeof(member));
    }
    if (size == 0) {
        *out_size = 0;
        return malloc(sizeof(member));
    }
    member *arr = calloc(size, sizeof(member));
    if (!arr) {
        printf("Error: Failed to allocate memory for members\n");
        *out_size = 0;
        return NULL;
    }
    for (int i = 0; i < size; ++i) {
        member *m = &arr[i];
        fread(m->first_name, CHAR_SMALL, 1, fp);
        fread(m->last_name, CHAR_SMALL, 1, fp);
        fread(m->email, CHAR_SMALL, 1, fp);
        fread(m->phone_number, CHAR_SMALL, 1, fp);
        fread(&m->dob, sizeof(date), 1, fp);
        fread(&m->time_created, sizeof(date), 1, fp);
        fread(&m->type, sizeof(memberType), 1, fp);
        fread(&m->account_available, sizeof(bool), 1, fp);
        fread(&m->loan.loan_flagged, sizeof(bool), 1, fp);
        fread(&m->loan.loan_index, sizeof(int), 1, fp);
        fread(&m->loan.loan_capacity, sizeof(int), 1, fp);
        if (m->loan.loan_capacity < 0 || m->loan.loan_capacity > 10000) {
            printf("Error: Invalid loan_capacity %d for member %d\n", m->loan.loan_capacity, i);
            free(arr);
            *out_size = 0;
            return NULL;
        }
        if (m->loan.loan_capacity > 0) {
            m->loan.loan_ids = malloc(m->loan.loan_capacity * sizeof(int));
            if (!m->loan.loan_ids) {
                printf("Error allocating loan_ids\n");
                free(arr);
                *out_size = 0;
                return NULL;
            }
            fread(m->loan.loan_ids, sizeof(int), m->loan.loan_capacity, fp);
        } else {
            m->loan.loan_ids = NULL;
        }
        if (m->type == STAFF) {
            fread(&m->o.staff.is_hired, sizeof(bool), 1, fp);
            fread(&m->o.staff.member_id, sizeof(int), 1, fp);
            fread(&m->o.staff.member_code, sizeof(int), 1, fp);
        } else if (m->type == AUTHOR) {
            fread(m->o.author.genre, CHAR_SMALL, 1, fp);
            fread(&m->o.author.dod, sizeof(date), 1, fp);
            fread(&m->o.author.is_alive, sizeof(bool), 1, fp);
        }
    }
    *out_size = size;
    return arr;
}

char *combine_with_space(const char *a, const char *b) {
  const int size = snprintf(NULL, 0, "%s %s", a, b);
  char *out = malloc(size + 1);
  sprintf(out, "%s %s", a, b);
  return out;
}

char *combine(const char *a, const char *b) {
  const int size = snprintf(NULL, 0, "%s%s", a, b);
  char *out = (char *)malloc(size + 1);
  sprintf(out, "%s%s", a, b);
  return out;
}

int dinit_db() {
  printf("Saving Database..\n");
  char *loans_path_char = combine(DATA_DIR, DATA_LOANS_FNAME);
  char *books_path_char = combine(DATA_DIR, DATA_BOOKS_FNAME);
  char *members_path_char = combine(DATA_DIR, DATA_MEMBERS_FNAME);

  FILE *loans_path = fopen(loans_path_char, "wb");
  FILE *books_path = fopen(books_path_char, "wb");
  FILE *members_path = fopen(members_path_char, "wb");

  write_loans(loans_path, db_loans, db_loans_index);
  write_books(books_path, db_books, db_books_index);
  write_members(members_path, db_members, db_members_index);

  printf("Wrote %d Loans (capacity: %d)\n", db_loans_index, db_loans_capacity);
  printf("Wrote %d Books (capacity: %d)\n", db_books_index, db_books_capacity);
  printf("Wrote %d members (capacity: %d)\n", db_members_index, db_members_capacity);

	free(loans_path_char);
	free(books_path_char);
	free(members_path_char);
  return 0;
}

int init_db() {
    printf("Importing Database..\n");
    char *loans_path_char = combine(DATA_DIR, DATA_LOANS_FNAME);
    char *books_path_char = combine(DATA_DIR, DATA_BOOKS_FNAME);
    char *members_path_char = combine(DATA_DIR, DATA_MEMBERS_FNAME);

    db_loans = NULL;
    db_books = NULL;
    db_members = NULL;

    FILE *loans_path = fopen(loans_path_char, "rb");
    FILE *books_path = fopen(books_path_char, "rb");
    FILE *members_path = fopen(members_path_char, "rb");

    bool error_flag = false;

    if (loans_path == NULL) {
        printf("Couldent open \"%s\"\n", loans_path_char);
        char *com = combine_with_space(MKFILE_COMMAND, loans_path_char);
        system(com);
        free(com);
        printf("Created \"%s\" file\n", loans_path_char);
        db_loans_capacity = DEFAULT_DB_CAPACITY;
        db_loans = malloc(db_loans_capacity * sizeof(loan));
        if (db_loans == NULL) {
            printf("db_loans could not be initialized\n");
            return -5;
        }
        db_loans_index = 0;
        error_flag = true;
    } else {
        db_loans = read_loans(loans_path, &db_loans_index);
        if (db_loans == NULL || db_loans_index == 0) {
            printf("db_loans dident load any data\ninitializing now...\n");
            db_loans_capacity = DEFAULT_DB_CAPACITY;
            db_loans = malloc(db_loans_capacity * sizeof(loan));
            if (db_loans == NULL) {
                printf("db_loans could not be initialized\n");
                return -5;
            }
        } else {
            db_loans_capacity = (db_loans_index + 1) * 2;
        }
        printf("Imported %d Loans (capacity: %d) from %s to db_loans\n", db_loans_index, db_loans_capacity, DATA_LOANS_FNAME);
    }

    if (books_path == NULL) {
        printf("Couldent open \"%s\"\n", books_path_char);
        char *com = combine_with_space(MKFILE_COMMAND, books_path_char);
        system(com);
        free(com);
        printf("Created \"%s\" file\n", books_path_char);
        db_books_capacity = DEFAULT_DB_CAPACITY;
        db_books = malloc(db_books_capacity * sizeof(book));
        if (db_books == NULL) {
            printf("db_books could not be initialized\n");
            return -5;
        }
        db_books_index = 0;
        error_flag = true;
    } else {
        db_books = read_books(books_path, &db_books_index);
        if (db_books == NULL || db_books_index == 0) {
            printf("db_books dident load any data\ninitializing now...\n");
            db_books_capacity = DEFAULT_DB_CAPACITY;
            db_books = malloc(db_books_capacity * sizeof(book));
            if (db_books == NULL) {
                printf("db_books could not be initialized\n");
                return -5;
            }
        } else {
            db_books_capacity = (db_books_index + 1) * 2;
        }
        printf("Imported %d Books (capacity: %d) from %s to db_loans\n", db_books_index, db_books_capacity, DATA_BOOKS_FNAME);
    }

    if (members_path == NULL) {
        printf("Couldent open \"%s\"\n", members_path_char);
        char *com = combine_with_space(MKFILE_COMMAND, members_path_char);
        system(com);
        free(com);
        printf("Created \"%s\" file\n", members_path_char);
        db_members_capacity = DEFAULT_DB_CAPACITY;
        db_members = malloc(db_members_capacity * sizeof(member));
        if (db_members == NULL) {
            printf("db_members could not be initialized\n");
            return -5;
        }
        db_members_index = 0;
        error_flag = true;
    } else {
        db_members = read_members(members_path, &db_members_index);
        if (db_members == NULL || db_members_index == 0) {
            printf("db_members dident load any data\ninitializing now...\n");
            db_members_capacity = DEFAULT_DB_CAPACITY;
            db_members = malloc(db_members_capacity * sizeof(member));
            if (db_members == NULL) {
                printf("db_members could not be initialized\n");
                return -5;
            }
        } else {
            db_members_capacity = (db_members_index + 1) * 2;
        }
        printf("Imported %d members (capacity: %d) from %s to db_members\n", db_members_index, db_members_capacity, DATA_MEMBERS_FNAME);
    }

    free(loans_path_char);
    free(books_path_char);
    free(members_path_char);

    if (error_flag) return 1;
    return 0;
}

int book_add(const book a) {
  if (db_books_index == db_books_capacity) {
    db_books_capacity *= 2;
    book *temp = realloc(db_books, db_books_capacity * sizeof(book));
    if (!temp) {
      printf("Error allocating memory for book\n");
      return -1;
    }
    db_books = temp;
  }

	if(db_books == NULL) printf("Error db_books is unininitialized\n");

  db_books[db_books_index] = a;
  db_books_index++;
	printf("created new book at index %d\n",db_books_index-1);
  return db_books_index - 1;
}

int loan_add(const loan a) {
  if (db_loans_index == db_loans_capacity) {
    db_loans_capacity *= 2;
    loan *temp = realloc(db_loans, db_loans_index * sizeof(loan));
    if (!temp) {
      printf("Error allocating memory for loans\n");
      return -1;
    }

    db_loans = temp;
  }

	if(db_loans == NULL){
		printf("Error db_loans is unininitialized\n");
		return -2;
	}

  db_loans[db_loans_index] = a;
  db_loans_index++;
	printf("created new loan at index %d\n",db_loans_index-1);
  return db_loans_index - 1;
}

int book_add_index(const book a, const int b) {
  if (b > db_books_index)
    return 0;

  db_books[b] = a;
  return 1;
}

int member_add_index(const member a, const int b) {
  if (b > db_members_index)
    return 0;

  db_members[b] = a;
  return 1;
}

int loan_add_index(const loan a, const int b) {
  if (b > db_loans_index)
    return 0;

  db_loans[b] = a;
  return 1;
}

date date_now(void) {
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  return (date){
      .year = (tm.tm_year + 1900),
      .month = tm.tm_mon,
      .day = tm.tm_mday,
      .hour = tm.tm_hour,
      .minute = tm.tm_min,
      .second = tm.tm_sec,
  };
}


void print_datetime(date a) {
  printf("%d:%d:%d, %d/%d/%d\n", a.hour, a.minute, a.second, a.day, a.month,
         a.year);
}

void loan_new(member *a, loan b){
	int loanid = loan_add(b);

	// initialize missing loans 
	if(a->loan.loan_ids == NULL) { 
		if (a->loan.loan_capacity <= 0)
			a->loan.loan_capacity = 2; // default amount of loans 

		a->loan.loan_ids = malloc(a->loan.loan_capacity * sizeof(int));
	}

	// reallocate loans 
	if(a->loan.loan_index == a->loan.loan_capacity){
		a->loan.loan_capacity *= 2;
		a->loan.loan_flagged = true; 
		int *temp = realloc(a->loan.loan_ids,a->loan.loan_capacity * sizeof(int));
		if(!temp){
			printf("Couldn't allocate memory for new loan\n");
			return;
		} else a->loan.loan_ids = temp;
	}

	// append loans 
	a->loan.loan_ids[a->loan.loan_index] = loanid;
	a->loan.loan_index++;
	return;
}

// unpayed and payed
int total_loan(member a){
	int out = 0;
	for(int i = 0; i < a.loan.loan_index;i++)
		out += db_loans[a.loan.loan_ids[i]].amount;
	return out;
}

void print_number(const int a){
	int b = snprintf(NULL,0,"%d",a);
	char c[b+1];
	sprintf(c,"%d",a);
	int d = strlen(c) % 3; // difference for modulo
	for(int i = 0; i < strlen(c); i++){
		if(i+d % 3 == 0) printf("%c,",c[i]);
		else printf("%c",c[i]);
	}
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
	date newdate = {.year = -1};
	
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

/*
member member_wizard(void){
	char first_name[CHAR_SMALL];
	char last_name[CHAR_SMALL];
	char email[CHAR_SMALL];
	char phone_number[CHAR_SMALL];
	
	// take string input
	input(first_name,CHAR_LARGE,"First Name: ");
	input(last_name,CHAR_LARGE,"Last Name: ");
	input(email,CHAR_SMALL,"Email: ");
	input(phone_number,CHAR_SMALL,"Phone Number: ");

	char type[CHAR_SMALL+1];
	input(type,CHAR_SMALL,"Enter Account type (MEMBER/author/staff): ");
	if(strcmp(type,"author")==0){
	}
	
	return (member){first_name,last_name,
		.email = email,
		.phone_number = phone_number,
	};
}
*/ 

int member_add(const member a) {
    if (db_members == NULL) {
        printf("Error: db_members is uninitialized\n");
        return -1;
    }
    if (db_members_index >= db_members_capacity) {
        db_members_capacity *= 2;
        printf("capacity of members has been changed to %d\n", db_members_capacity);
        member *temp = realloc(db_members, db_members_capacity * sizeof(member));
        if (!temp) {
            printf("Error allocating memory for members\n");
            return -1;
        }
        db_members = temp;
    }
    db_members[db_members_index] = a;

    if (a.loan.loan_capacity > 0 && a.loan.loan_ids != NULL) {
        if (a.loan.loan_capacity > 10000) { // Arbitrary limit to prevent invalid sizes
            printf("Error: Invalid loan_capacity %d\n", a.loan.loan_capacity);
            return -1;
        }
        db_members[db_members_index].loan.loan_ids = malloc(a.loan.loan_capacity * sizeof(int));
        if (!db_members[db_members_index].loan.loan_ids) {
            printf("Error allocating memory for loan_ids\n");
            return -1;
        }
        memcpy(db_members[db_members_index].loan.loan_ids, a.loan.loan_ids, a.loan.loan_capacity * sizeof(int));
    } else {
        db_members[db_members_index].loan.loan_ids = NULL;
    }
    db_members_index++;
    printf("Created new member at index %i\n", db_members_index - 1);
    return db_members_index - 1;
}
#include "lib.h"

#ifdef __unix__
	#include <dirent.h>
	#include <unistd.h> 
	#include <termios.h>
	
	int file_exist(const char *path) {
  	if (access(path, F_OK) == 0)
    	return 1;
  	return 0;
	}

	int dir_exist(const char *path) {
  	DIR *dir = opendir(path);
  	if (dir) {
    	closedir(dir);
    	return 1;
  	}
  	return 0;
	}


	int achar(){
		static struct termios oldt, newt;
		tcgetattr(STDIN_FILENO,&oldt);
		newt=oldt;
		newt.c_lflag &= ~(ICANON | ECHO );
		tcsetattr(STDIN_FILENO,TCSANOW,&newt);
		int ch=getchar();
		tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
		return ch;
	}


#elif _WIN32
	#include <io.h>
	#include <windows.h>

	int file_exist(const char *path)
		if (_access(fname, 0) == 0) return 1;
		return 0;
	}

	int dir_exist(const char *path) {
    DWORD attrib = GetFileAttributesA(path);
    if (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY)) {
        return 1;
  	}
  	  return 0; 
	}

	int achar() {
		HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
		DWORD mode, bytesRead;
		INPUT_RECORD ir;
  	KEY_EVENT_RECORD ker;
  	GetConsoleMode(hStdin, &mode);
  	SetConsoleMode(hStdin, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
  	ReadConsoleInput(hStdin, &ir, 1, &bytesRead);
		if (ir.EventType == KEY_EVENT && ir.Event.KeyEvent.bKeyDown) {
  		ker = ir.Event.KeyEvent;
  		char ch = ker.uChar.AsciiChar;
    	if (ch == '\r') { 
      	ch = '\n';
    	} else if (ker.wVirtualKeyCode >= VK_UP && ker.wVirtualKeyCode <= VK_DOWN) {
    		switch (ker.wVirtualKeyCode) {
      		case VK_UP:    ch = 'A'; break;
        	case VK_DOWN:  ch = 'B'; break;
        	case VK_RIGHT: ch = 'C'; break;
        	case VK_LEFT:  ch = 'D'; break;
      	}
    	}
    	SetConsoleMode(hStdin, mode);
    	return (unsigned char)ch;
  	}
  	SetConsoleMode(hStdin, mode);
  	return -1;
	}

#endif
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "plib.h"
/* TODO
 * add type based conversion using universal var.
 * add functions and toggle pointers.
 * add error vars for better readability
 * ^ ontop of that add a enum to stringifyer

 * so that i can print the error on exit aswell
 * as having the actual code from the enum
 * add verbose/debug mode
 * add quiet mode
 * write to plib.h
 * remove deps (string.h)
 * add better logging function that supports errors and stuff.
 * get a life :(
 *
 * function to see if ALL of the created args are triggered.
 */

typedef enum {
  ERROR,
  VERBOSE,
	VERBOSE_ERROR,
} mode;

argument *argument_list;
int argument_list_index = 0;
int argument_list_capacity = 0;
const int noquiet = 1;

// macro definition makes the function automatically take in the line and file.
#define printc(a, c, ...) printc_implicit(a, __LINE__, __FILE__, c, ##__VA_ARGS__)

// basic print wrapper.
int printc_implicit(mode mode, const int LINE, const char *FILE, const char *format, ...) {
  if (noquiet == 0) return 0;
  if (noquiet == 1 && mode == 1) return 0;
  if (noquiet >= 2) return 0;
	if(noquiet == 1 && mode == 2) return 0;

  va_list args;
  printf("[%s] %s@%d: ", mode ? "Verbose" : "Error", FILE, LINE);
  va_start(args, format);
  vprintf(format, args);
  va_end(args);

  return 0;
}

char *strsep(char **stringp, const char *delim) {
  if (*stringp == NULL)
    return NULL;

  char *token_start = *stringp;
  *stringp = strpbrk(token_start, delim);
  if (*stringp) {
    **stringp = '\0';
    (*stringp)++;
  }
  return token_start;
}

// Basic checker function to ensure that
// 1. the argument_list is initialized
// and 2. that the argument list has
// enough space to hold another item.
int validate_argument_list() {
  // init argument list
  if (argument_list_capacity == 0) {
    argument_list_capacity = 5;
    argument_list = malloc(argument_list_capacity * sizeof(argument));
    if (!argument_list) {
      printc(ERROR, "unable to initialize argument_list (malloc)\n");
      return 1;
    }
  }

  // re-allocate argument_list
  if (argument_list_capacity == argument_list_index) {
    argument_list_capacity *= 2;
    argument *temp =
        realloc(argument_list, argument_list_capacity * sizeof(argument));
    if (!temp) {
      printc(ERROR, "unable to re-allocate argument_list (malloc)\n");
      free(argument_list);
      return 1;
    } else
      argument_list = temp;
  }

  if (argument_list == NULL)
    return 1;

  return 0;
}

// set an argument ( used for later in the
// process_arguments function )
int set_argument(argument **pointer_return, set_argument_options options) {
  if (validate_argument_list() != 0)
    return 1;
  argument *local_argument = &argument_list[argument_list_index];

  // zero it out

  // set values
  local_argument->description = options.DESCRIPTION;
  local_argument->catagory = options.FLAG_CATAGORY;
  local_argument->name = options.FLAG_NAME;
  local_argument->takes_value = options.takes_value;
  local_argument->triggered = 0;
  local_argument->value = NULL;
  argument_list_index++;
  *pointer_return = local_argument;
  return 0;
}

// EXPERIMENTAL
int set_bulk_argument(const char *arguments[], const int argument_size, const int input_type) {
  for (int i = 0; i > argument_size; i++) {
    argument *local_argument = NULL;
    set_argument(&local_argument, (set_argument_options){
    	.FLAG_NAME = arguments[i],
      .takes_value = input_type,
    });

    printc(VERBOSE, "added bulk argument: %s", arguments[i]);
  }
  return 1;
}

// Print all of the arguments that have
// been set so far, this will be called
// if an argument is non-existant or if
// and invalid argument is ran.
void phelp() {
  if (argument_list == NULL) return;

	int longest = 0;
	int longest_catagory = 0;

	for(int i = 0;i <argument_list_index; i++){
		// get longest name 
		if(longest < strlen(argument_list[i].name))
			longest = strlen(argument_list[i].name);

		// get longest catagory
		if(longest_catagory < strlen(argument_list[i].catagory))
			longest_catagory = strlen(argument_list[i].catagory);
	}

  printf("Options:\n");
	printf("Name, Catagory, Description\n");
  for (int i = 0; i < argument_list_index; i++) {
    printf("%s", argument_list[i].name);
		for(int j = 0;j <longest - strlen(argument_list[i].name);j++) printf(" ");
		printf(" | %s",argument_list[i].catagory);
		for(int j = 0;j<longest_catagory - strlen(argument_list[i].catagory);j++) printf(" ");
		printf(" | %s\n",argument_list[i].description);
  }
}

// Deallocate the memory from argument_list
// this will be useful later when we have
// embedded pointers and functions that take
// up ram on the argument.
void dinit_argument_list() {
  if (validate_argument_list() == 0) {
    for (int i = 0; i < argument_list_index; i++) {
      if (argument_list[i].value) {
        printc(VERBOSE, "free'd %lu bytes\n", strlen(argument_list[i].value));
        free(argument_list[i].value);
      }
    }
    printc(VERBOSE, "free'd argument_list\n");
    free(argument_list);
  }
}

int argument_exists(const char *name) {
  for (int i = 0; i < argument_list_index; i++)
    if (strcmp(argument_list[i].name, name) == 0)
      return i; // argument name found in argument_list

  return -1; // argument was not found in argument_list
}

// go through each argument and parse them
// by checking the record with argument_list
int parse_arguments(const int argc, const char *argv[]) {
  if (argc == 1) {
    printc(VERBOSE_ERROR, "No arguments given\n");
    return 1;
  }
  for (int i = 1; i < argc; i++) {
    if (!argv[i]) {
      printc(VERBOSE_ERROR, "NULL argument at index %d\n", i);
      continue;
    }

    // handle -- flag
    if (strcmp(argv[i], "--") == 0) {
      const int idx = argument_exists("--");
      if (idx != -1) {
        // the -- flag has been defined by user
        argument *local = &argument_list[idx];

        // Start collecting arguments after "--"
        size_t totalLen = 0;
        for (int j = i + 1; j < argc; j++)
          totalLen += strlen(argv[j]) + 1; // space or null terminator

        local->value = malloc(totalLen + 1);
        if (!local->value) {
          printc(ERROR, "Couldn't allocate memory for -- flag\n");
          return -1;
        }

        local->triggered = 1;
        local->value[0] = '\0';
        for (int j = i + 1; j < argc; j++) {
          strcat(local->value, argv[j]);
          if (j < argc - 1) strcat(local->value, " ");
        }

        return 0;
      }
    }

    char *str, *to_free, *token;
    const char *arg = argv[i];
    char *key = NULL, *value = NULL;
    int token_count = 0;
    int return_code = 0;

    to_free = str = strdup(arg);
    if (!str) {
      printc(ERROR, "Memory allocation failed for argument '%s'\n", arg);
      return 1;
    }

    while ((token = strsep(&str, "="))) {
      if (strlen(token) == 0)
        continue;

      if (token_count == 0)
        key = token;
      else if (token_count == 1)
        value = token;
      else {
        return_code = 1;
        break;
      }
      token_count++;
    }

    // handle the arguments
    if (return_code == 1)
      printc(VERBOSE_ERROR, "too many '=' in argument: '%s'\n", arg);
    else if (!key || strlen(key) == 0) {
      return_code = 1;
      printc(VERBOSE_ERROR, "invalid or empty key in '%s'\n", arg);
    } else {
      const int argument_index = argument_exists(key);
      if (argument_index != -1) {
        argument *local_argument = &argument_list[argument_index];
        if (!value || strlen(value) == 0) {
          /* a key has been provided but it has
           * no value, this is good for if you have
           * void flags like --help. */
          if (local_argument->takes_value != 0) {
            printc(VERBOSE_ERROR, "argument '%s' takes a value but none was provided\n",
                   key);
            return_code = 1;
          }

          local_argument->triggered = 1;
        } else {
          /* a key has been provided and a value
           * has also been provided. */
          if (local_argument->takes_value != 1) {
            printc(VERBOSE_ERROR, "argument '%s' does not take a value but '%s' was provided\n", key, value);
            return_code = 1;
          }

          // set the value if needed
          local_argument->value = malloc(strlen(value) + 1);
          if (local_argument->value) {
            strcpy(local_argument->value, value);
            local_argument->triggered = 1;
          } else printc(ERROR, "couldent allocate memory for argument '%s' value\n", key);
        }
      } else printc(ERROR, "argument '%s' not found\n", key);
    }

    free(to_free);
    if (return_code != 0) return return_code;
  }

  return 0;
}

// get bool if argument has been run or not,
// useful for void argument flags like --help
int argument_run(const argument *local) {
  if (validate_argument_list() != 0) return -1;
  if (!local) return 1;
  if (local->triggered) return 0;
  return 1;
}

char *argument_value(const argument *local) {
  if (validate_argument_list() != 0) {
    printc(ERROR, "argument_list validation failed\n");
    return "";
  }

  if (local->triggered) {
    if (local->value) return local->value;
    else {
      printc(ERROR, "argument '%s' is triggered but has no value\n", local->name);
      return "";
    }

  } else {
    printc(ERROR, "argument '%s' does not have a value\n", local->name);
    return "";
  }

  printc(ERROR, "argument '%s' has not been run\n", local->name);
  printc(ERROR, "^--> use '*argument_run(argument *)'\n");
  return "";
}

// EXPERIMENTAL
int is_all_triggered() {
  if (validate_argument_list() != 0) return -1;
  for (int i = 0; i < argument_list_index; i++) {
    const argument *local = &argument_list[i];
    if (argument_run(local) != 0) {
      printc(VERBOSE, "%s flag not triggered", local->name);
      return 1;
    }
  }
  return 0;
}
/*
TODO: 
for some reason the program crashes on FIRST write, i think this has something to do with 
the members write function but im not sure. 

finish writing the wizard functions.
*/

// 3rd party imports
#include "lib/data.h"
#include "lib/lib.h"
#include "lib/plib.h"
#include "db.c" // actual database file
//#include "ui.c" // interface file

// standard librarys 
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>


void quit(int code){
	printf("\rexiting program with code %d ..\n",code);
	dinit_argument_list();
	exit(code);
}

// status of the database and handles flags.
int main(const int argument_count, const char *argument_list[]){ 
	if(PLATFORM_NOT_SUPPORTED){
		printf("your platform is not supported\neither use _WIN32 or __unix__ platforms\n");
		exit(-1);
	}

	argument* clean = NULL;
	set_argument(&clean,(set_argument_options){
			.FLAG_NAME = "--clean",
			.FLAG_CATAGORY = "base",
			.DESCRIPTION = "Destroy old data files (wipe the database)",
			.takes_value = 0,
			});

	argument *run = NULL;
	set_argument(&run,(set_argument_options){
			.FLAG_NAME = "--run",
			.FLAG_CATAGORY = "base",
			.DESCRIPTION = "Run the database code",
			.takes_value = 0,
			});

	argument *view = NULL;
	set_argument(&view, (set_argument_options){
			.FLAG_NAME = "--view",
			.FLAG_CATAGORY = "base",
			.DESCRIPTION = "View database information",
			.takes_value = 0,
			});

	// process arguments
	if(parse_arguments(argument_count,argument_list)==0){

		// catch signals
		signal(SIGINT,quit);

		// parse system arguments
		if(argument_run(clean)==0){
			if(!dir_exist(DATA_DIR)){
				printf("Database directoy dosent exist.\nThis means ther is nothing to clean\n");
				quit(0);
			}	

			char buff[SECURITY_CODE_LENGTH+1];

			// input prompt confirmation
			input(buff,SECURITY_CODE_LENGTH+1,"Do you really want to delete the database? (y/N): ");

			if(strcmp(buff,"y")==0 || strcmp(buff,"Y")==0){
				printf("To Confirm please write the following phrase into the input\n"
					"As soon as you press enter after putting in the folloing the database will be deleted\n");

				// generate confirmation code
				char rand[SECURITY_CODE_LENGTH + 1];
				for(int i=0;i<SECURITY_CODE_LENGTH;i++)
					printf("%c",rand[i] = random_char());
				printf("\n");
				rand[SECURITY_CODE_LENGTH] = '\0';

				// input for secuirty question
				input(buff,SECURITY_CODE_LENGTH+1,": ");

				if(strcmp((char *)buff,(char *)rand)==0){

					// delete the database folder
					char *com = combine_with_space(DELETE_COMMAND,DATA_DIR);
					system(com);
					free(com);
					printf("database deleted!\n");
				} else {
					printf("Wrong code entered\n");
					quit(1);
				}
			}
		}

		if(!dir_exist(DATA_DIR)){
			printf("Couldent open \"%s\"\n",DATA_DIR);
			// create the database folder
			char *com = combine_with_space(MKDIR_COMMAND,DATA_DIR);
			system(com);
			free(com);
			printf("Created \"%s\" dir\n",DATA_DIR);
		}
		
		// import the database
		switch(init_db()){
			case 1:
				// re-import the database 
				if(init_db() == 1)
					printf("Error occured with database, unsure of cause\n");
					exit(1);
				break;

			case -5: 
				printf("fatal database import error\n");
				exit(-5);
				break;

			default: 
				break;
		}

		if(argument_run(run)==0){
			printf("DATABASE OUTPUT:\n\n"); 
			int ret = database();
			printf("\nEND OF DATABASE OUTPUT (exited with code %d)\n",ret);
			fflush(stdout);
		}

		// save the database 
		dinit_db();
		free(db_members);
    free(db_loans);
    free(db_books);
    db_members = NULL;
    db_loans = NULL;
    db_books = NULL;
	} else phelp();
	quit(0);
}
#include "lib/hlib/lib/ansi.h"
#include "lib/lib.h"
#include <signal.h> 
#include <stdlib.h>
#include <string.h>
#include <stdio.h> 

#define DEFAULT_PROMPT "Use the up/down arrows to navigate, press enter to submit entry\n"

void ui_quit(int code){
	show_cursor();
	exit(code);
}

#define ui_m(array, prompt) ui_menu(array,sizeof(array)/sizeof(array[0]),prompt);
int ui_menu(const char *array[], const int size, const char *prompt){
	hide_cursor();
	
	int selected = 0; 
	int longest = 0;

	// calulate the longest item in the list: 
	for(int i = 0; i < size; i++)
		if(strlen(array[i]) > longest) longest = strlen(array[i]);	

	while(1){
		clear();
		printf("%s",prompt);
		
		// draw the menu 
		for(int i = 0; i < size; i++){
			printf(" %2d. %s",i+1,array[i]);
			
			// draw some white space 
			for(int j = 0; j < longest - strlen(array[i]); ++j) printf(" ");
			if(i == selected) printf(" <\n");
			else printf("  \n");
		}


		// handle the input
		int ch = achar();
		switch(ch){
			case 'B':
				if(selected==size-1) selected = 0;
				else selected++;
				break;
			
			case 'A':
				if(selected==0) selected=size-1;
				else selected--;
				break;

			case 10:
				show_cursor();
				return selected;
		}
	}

	show_cursor();
	return -1;
}



int ui_main(){
	// main menu
	const char *main_menu[] = {
		"Create Data",
		"Read Data",
		"Update Data", 
		"Delete Data",
	};

	const char *create[] = {
		"Member",
		"Book",
		"Loan",
	};

	const char *read[] = {
		"Members",

	};


	int ret = ui_m(main_menu,"Please select the database macro\n");
	return 0;
}


int ui_test(){
	signal(SIGINT,ui_quit);

	// define items for the menu
	const char *list[] = {
		"one",
		"two",
		"three",
		"four",
		"five",
		"six",
		"seven",
		"eight",
		"nine",
		"ten",
	};
	
	// use the menu function to ask the user for input
	const int sel = ui_m(list,DEFAULT_PROMPT);

	// display the chosen item
	printf("You picked %s!\n",list[sel]);
	
	return 0;
}

#ifndef DATA_TYPES_H
#define DATA_TYPES_H
#include <stdbool.h> // for (true|false) and boolian type.

#define CHAR_LARGE 1028
#define CHAR_SMALL 256
#define MAX_LOANS 3

typedef enum {
  MEMBER = 0,
  AUTHOR = 1,
  STAFF = 2,
} memberType;

typedef struct {
  int month;
  int year;
  int day;
  int hour;
  int minute;
  int second;
} date;

// covers loans and all associated data
typedef struct {
  int amount;       // fee 
  date issued;      // when the book was taken out 
  date returned;    // when the book was returned
  date return_date; // when the book NEEDs to be returned by
	int bookid;       // id to the book.
	bool active;      // if the loan has been fulfilled.
	char note[CHAR_LARGE]; // note of loan
  union {
    bool payed;
    bool covered;
  } is;
} loan;

// covers members staff and authors
typedef struct {
	// contact info: 
  char first_name[CHAR_SMALL];
  char last_name[CHAR_SMALL];
  char email[CHAR_SMALL];
  char phone_number[CHAR_SMALL];

	// extra info:
  date dob;
  date time_created;
	bool account_available;
	bool account_to_delete; /* this isent included in other structs because loans 
													   have the is union and books have the available union. */

	// loan data: 
  struct {
  	bool loan_flagged; // flag the loan (might be useful in backend) 
    int *loan_ids; // this is an array of ids refering to a loan in the db_loan array
		int loan_index;
		int loan_capacity;
  } loan;

	// type info
  memberType type;
  union {
    struct {
      bool is_hired;
      int member_id;
      int member_code;
    } staff;
    struct {
      char genre[CHAR_SMALL];
      date dod;
      bool is_alive;
    } author;
  } o;
} member;

// covers all books
typedef struct {
  char title[CHAR_LARGE]; 
  int id_author; // id of the member  
  date publication_date; 
  char genre[CHAR_SMALL];
  char ISBN[15];
  int available; // how many of the book the library has not on loan 
	int count; // how many of that book the library has 
} book;

/* NOTE: This is the perfect set of structures, there is no double up data 
 * eg there is no member list in each book struct, this is because using 
 * a more hands on object based database means that you can just implement 
 * any features that you want to.This also means less bloating of things that 
 * you do not need. 
 */

#endif // DATA_TYPES_H
#define DATA_TYPES_H
#ifndef ANSI_H
#define ANSI_H
#define ESCAPE_CODE "\033"
#define ESC ESCAPE_CODE
#define ansi(seq) printf("%s%s", ESC, seq);
#define activate_terminal_buffer() ansi("[?1049h");
#define deactivate_terminal_buffer() ansi("[?1049l");
#define enable_mouse_reporting_ansi() ansi("[?1003h");
#define disable_mouse_reporting_ansi() ansi("[?1003l");
#define hide_cursor() ansi("[?25l");
#define show_cursor() ansi("[?25h");

// Special key codes (starting at 100+ to avoid overlap with ASCII)
#define KEY_ARROW_UP 100
#define KEY_ARROW_DOWN 101
#define KEY_ARROW_RIGHT 102
#define KEY_ARROW_LEFT 103
#define KEY_HOME 104
#define KEY_INSERT 105
#define KEY_DELETE 106
#define KEY_END 107
#define KEY_PAGE_UP 108
#define KEY_PAGE_DOWN 109
#define KEY_F1 110
#define KEY_F2 111
#define KEY_F3 112
#define KEY_F4 113
#define KEY_F5 114
#define KEY_F6 115
#define KEY_F7 116
#define KEY_F8 117
#define KEY_F9 118
#define KEY_F10 119
#define KEY_F11 120
#define KEY_F12 121

#define clear() printf("\033[H\033[J")
#define gotoxy(x, y)                                                           \
  printf("\033[%d;%dH", (y + 1),                                               \
         (x + 1)) // plus one because 1;1 is the same as 0;0
// #define clear_screen() ansi("[2J") DEPRICATED
#define cursor_home() ansi("[H")
#endif // ANSI_H
#define ANSI_H
#ifndef DATABASE_LIB 
#define DATABASE_LIB

// for the datatypes from data.h 
#include "data.h"

// for bool, true and false.
#include <stdio.h>

// for free 
#include <stdlib.h>
#define DEFAULT_DB_CAPACITY 5
// constants
#define SECURITY_CODE_LENGTH 3 // read ../main.c 
#define DATA_DIR "./db/" // folder name of the database 
#define DATA_LOANS_FNAME "loans.bin"
#define DATA_BOOKS_FNAME "books.bin"
#define DATA_MEMBERS_FNAME "members.bin"

// macros
#define free free

// global variables for the database.
extern loan *db_loans;
extern book *db_books;
extern member *db_members;

// used for memory allocation
extern int db_loans_index; 
extern int db_books_index;
extern int db_members_index;
extern int db_loans_capacity;
extern int db_books_capacity;
extern int db_members_capacity;

// return the current date in the date format (data.h)
date date_now(void);

// return a random printable character
char random_char(void);

// take input (buffer is a pointer to the input)
void input(char *buffer, int size, char* prompt);

// combine two strings with a space inbetween
char *combine_with_space(const char *a, const char *b); 

// combine two strings without a space in the middle
char *combine(const char *a, const char *b);



/* SAVE: 
 * this function will automatically move all of the local 
 * datbase to the constant storage location on disk, this 
 * is done automatically on when the db.c file has exited.
 * */
int dinit_db(void);

/* WRITE: 
 * these functions write the ram to the disk, this is used 
 * in saving the database as it lets us save a copy of the 
 * programs data which is then imported later on. */
void write_loans(FILE *fp, loan *arr, int count);
void write_books(FILE *fp, book *arr, int count);
void members_write(FILE *a, member *arr,const int size);


/* IMPORT: 
 * uses the read_* functions to import data from the disk 
 * directly into the program, this is executed automatically 
 * before the db.c file is executed. in theory the database 
 * handles all I/O operations automatically. */
int init_db(void);

/* CREATE: 
 * these functions will append an item in the 
 * db_* array, these functions will return the 
 * index at which the new item was appended, 
 * this is used in linking data between more data
 * */
int book_add(const book a);
int member_add(const member a);
int loan_add(const loan a);

/* READ: 
 * the following are oposite versions of the 
 * write_* functions, these functions simply 
 * use fwrite to move the storage location 
 * of the database from the programs RAM stack 
 * to the local disk storage, this allows the 
 * database to have constant storage and 
 * save information. */ 
loan *read_loans(FILE *fp, int *out_count);
book *read_books(FILE *fp, int *out_count); 
member *read_members(FILE *a, int *out_size);

/* members contain strings that have to 
 * be allocated so this function is a helper 
 * to free that memory */
void free_member(member *m);

// print the date and time 
void print_datetime(date a);

/* this function is a wrapper for the loan_add function 
 * all this function does is it takes in a member a loan 
 * it then uses the loan_add function to add the new loan 
 * to the db_loans array then it links the loan to the member 
 * by adding the new loans id to the members loan_ids array. 
 * this would be around 17 lines of code to do without the 
 * function so this function is quite important. */ 
void loan_new(member *a, loan b);

// show the total loan fee owing from a user.
int total_loan(member a);

// print a number with commas
void print_number(const int a);

// convert a string to an int ( with error checking );
int eatoi(const char *a);

/* the wizard commands are used to prompt the user 
 * for realtime data. */
date date_wizard(void);
member member_wizard(void);

// return a pointer to a member using an memberid.
member *id_to_member_ptr(const int a);

// get the name of a member "<first_name> <last_name>"
char *member_name(member a);

// copy a member (non pointer) from an id
member id_to_member(const int a);

#endif // DATABASE_LIB 

// platform code 
#ifndef PLATFORM_H
	// platform selection: 
	#ifdef __unix__
		// if on unix: 
		#define DELETE_COMMAND "rm -rf"
		#define MKDIR_COMMAND "mkdir"
		#define MKFILE_COMMAND "touch"
		#define PLATFORM_NOT_SUPPORTED 0
	#elif _WIN32
		// if on windows
		#define DELETE_DOMMAND "del /s /q"
		#define MKDIR_COMMAND "mkdir" // or md
		#define MKFILE_COMMAND "echo '' >"
	#else 
		#define PLATFORM_NOT_SUPPORTED 1
	#endif
	
	int dir_exist(const char *path);
	int file_exist(const char *path); 
	int achar(void);
#endif // PLATFORM_H
#ifndef PLIB3_H
#define PLIB3_H

// Data types
typedef struct argument {
  const char *name;
  const char *description;
  const char *catagory;
  char *value;
  int takes_value;
  int triggered;
} argument;

typedef struct set_argument_options {
  const char *FLAG_NAME;
  const char *DESCRIPTION;
  const char *FLAG_CATAGORY;
  const int takes_value;
} set_argument_options;

typedef enum {
  NO_INPUT,
  TAKE_INPUT,
} set_argument_enum_values;

// Functions
int set_argument(argument **pointer_return, set_argument_options options);
void phelp(void);
void dinit_argument_list(void);
int argument_exists(const char *name);
int parse_arguments(const int argc, const char *argv[]);
int argument_run(const argument *local);
char* argument_value(const argument *local);
int set_bulk_argument(const char *arguments[],const int argument_size, const int input_type);
int is_all_triggered(void);

// Global vars
extern argument *argument_list;
extern int argument_list_index;
extern int argument_list_capacity;


#endif // PLIB3_H 
