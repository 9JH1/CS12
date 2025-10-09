#include "lib/data.h"
#include "lib/lib.h"
#include "ui.c"
#include <ctype.h>
#include <unistd.h>

int print_book_data(int book_index) {
  if (book_index < 0 || book_index >= db_books_index) {
    printf("Invalid book index: %d\n", book_index);
    return -1;
  }

  const int size = 7;
  char *key[size], *val[size];
  book book_cur = db_books[book_index];

  if (book_cur.id_author < 0 || book_cur.id_author >= db_members_index) {
    printf("Invalid author index: %d\n", book_cur.id_author);
    return -1;
  }

  for (int i = 0; i < size; i++) {
    key[i] = malloc(COL_SIZE * sizeof(char));
    val[i] = malloc(COL_SIZE_2 * sizeof(char));
    if (!key[i] || !val[i]) {
      printf("Memory allocation failed\n");
      for (int j = 0; j < i; j++) {
        free(key[j]);
        free(val[j]);
      }
      return -1;
    }
  }

  // Populate fields using strcpy and strcat
  strcpy(key[0], "title: ");
  strcat(key[0], book_cur.title);
  strcpy(val[0], "Book title");

  strcpy(key[1], "author: ");
  strcat(key[1], db_members[book_cur.id_author].first_name);
  strcat(key[1], " ");
  strcat(key[1], db_members[book_cur.id_author].last_name);
  strcpy(val[1], "Book author");

  strcpy(key[2], "ISBN: ");
  strcat(key[2], book_cur.ISBN);
  strcpy(val[2], "International Standard Book Number");

  strcpy(key[3], "genre: ");
  strcat(key[3], book_cur.genre);
  strcpy(val[3], "Book genre");

  char date_str[16];
  snprintf(date_str, sizeof(date_str), "%d/%d/%d",
           book_cur.publication_date.day, book_cur.publication_date.month,
           book_cur.publication_date.year);
  strcpy(key[4], "publication_date: ");
  strcat(key[4], date_str);
  strcpy(val[4], "Publication date");

  char num_str[16];
  snprintf(num_str, sizeof(num_str), "%d", book_cur.available);
  strcpy(key[5], "available: ");
  strcat(key[5], num_str);
  strcpy(val[5], "Number of books available for loan");

  snprintf(num_str, sizeof(num_str), "%d", book_cur.count);
  strcpy(key[6], "count: ");
  strcat(key[6], num_str);
  strcpy(val[6], "Total count of this book");

  int ret = ui_menu((const char **)key, size, (const char **)val, "View books");

  for (int i = 0; i < size; i++) {
    free(key[i]);
    free(val[i]);
  }

  return ret;
}

void print_loan_data(loan loan_cur) {
  printf("amount (owed): $%d\n", loan_cur.amount);
  printf("issued: %d/%d/%d - %d:%d:%d\n", loan_cur.issued.day,
         loan_cur.issued.month, loan_cur.issued.year, loan_cur.issued.hour,
         loan_cur.issued.minute, loan_cur.issued.second);
  printf("return_date (due date): %d/%d/%d - %d:%d:%d\n",
         loan_cur.return_date.day, loan_cur.return_date.month,
         loan_cur.return_date.year, loan_cur.return_date.hour,
         loan_cur.return_date.minute, loan_cur.return_date.second);

  printf("returned (when user returned book): %d/%d/%d - %d:%d:%d\n",
         loan_cur.returned.day, loan_cur.returned.month, loan_cur.returned.year,
         loan_cur.returned.hour, loan_cur.returned.minute,
         loan_cur.returned.second);

  printf("bookid %d\n", loan_cur.bookid);
  printf(
      "active (is the loan currently not returned) %d ( 0 = false 1 = true)\n",
      loan_cur.active);
  printf("is.payed (has the loan be payed) %d\n", loan_cur.is.payed);
  printf("is.covered (has the loan been payed) %d\n", loan_cur.is.covered);
  printf("note: %s\n", loan_cur.note);
  printf("press any key to view book information\n");
  achar();
  print_book_data(loan_cur.bookid);
  printf("\n");
}

int print_member_data(int member_index) {
  member member_cur = db_members[member_index];
  const int size = 9; // Increased to handle all possible cases
  char *key[size], *val[size];
  int end_size = 0;

  for (int i = 0; i < size; i++) {
    key[i] = malloc(COL_SIZE * sizeof(char));
    val[i] = malloc(COL_SIZE_2 * sizeof(char));
    if (!key[i] || !val[i]) {
      printf("Memory allocation failed\n");
      for (int j = 0; j < i; j++) {
        free(key[j]);
        free(val[j]);
      }
      return -1;
    }
  }

  strcpy(key[0], "first_name,last_name: ");
  strcat(key[0], member_cur.first_name);
  strcat(key[0], " ");
  strcat(key[0], member_cur.last_name);
  strcpy(val[0], "First and last name");

  char date_str[16];
  snprintf(date_str, sizeof(date_str), "%d/%d/%d", member_cur.dob.day,
           member_cur.dob.month, member_cur.dob.year);
  strcpy(key[1], "dob: ");
  strcat(key[1], date_str);
  strcpy(val[1], "Date of birth");

  strcpy(key[2], "email: ");
  strcat(key[2], member_cur.email);
  strcpy(val[2], "Member Email");

  strcpy(key[3], "phone_number: ");
  strcat(key[3], member_cur.phone_number);
  strcpy(val[3], "Member phone number");

  strcpy(key[4], "account_to_delete: ");
  strcat(key[4], member_cur.account_available ? "False" : "True");
  strcpy(val[4], "Is the account to be deleted?");

  if (member_cur.type == STAFF) {
    strcpy(key[5], "type: STAFF");
    strcpy(val[5], "Type of member");

    char num_str[16];
    snprintf(num_str, sizeof(num_str), "%d", member_cur.o.staff.member_code);
    strcpy(key[6], "member_code: ");
    strcat(key[6], num_str);
    strcpy(val[6], "Non-unique staff-only code");

    snprintf(num_str, sizeof(num_str), "%d", member_cur.o.staff.member_id);
    strcpy(key[7], "staff_id: ");
    strcat(key[7], num_str);
    strcpy(val[7], "Unique staff identifier");

    strcpy(key[8], "is_hired: ");
    strcat(key[8], member_cur.o.staff.is_hired ? "True" : "False");
    strcpy(val[8], "Is the staff member hired?");
    end_size = 9;
  } else if (member_cur.type == AUTHOR) {
    strcpy(key[5], "type: AUTHOR");
    strcpy(val[5], "Type of member");

    strcpy(key[6], "genre: ");
    strcat(key[6], member_cur.o.author.genre);
    strcpy(val[6], "Authors average genre");

    if (member_cur.o.author.is_alive) {
      strcpy(key[7], "dod: N/A");
    } else {
      snprintf(date_str, sizeof(date_str), "%d/%d/%d",
               member_cur.o.author.dod.day, member_cur.o.author.dod.month,
               member_cur.o.author.dod.year);
      strcpy(key[7], "dod: ");
      strcat(key[7], date_str);
    }
    strcpy(val[7], "Date of death");

    int count = 0;
    for (int i = 0; i < db_books_index; i++)
      if (db_books[i].id_author == member_index)
        count++;

    char count_str[16];
    snprintf(count_str, sizeof(count_str), "%d", count);
    strcpy(key[8], "Author has ");
    strcat(key[8], count_str);
    strcat(key[8], " books");
    strcpy(val[8], "Author book count");
    end_size = 9;
  } else {
    strcpy(key[5], "type: MEMBER");
    strcpy(val[5], "Type of member");
    end_size = 6;
  }

  int ret = ui_menu((const char **)key, end_size, (const char **)val,
                    "Viewing Member Data");

  for (int i = 0; i < size; i++) {
    free(key[i]);
    free(val[i]);
  }

  return ret;
}

// dont forget to free!
char *lower(const char *in) {
  if (in == NULL || *in == '\0') {
    char *out = malloc(1);
    if (out == NULL) {
      return NULL;
    }
    out[0] = '\0';
    return out;
  }

  size_t in_s = strlen(in);
  char *out = malloc((in_s + 1) * sizeof(char));
  if (out == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < in_s; i++) {
    if (isalpha((unsigned char)in[i])) {
      out[i] = tolower((unsigned char)in[i]);
    } else {
      out[i] = in[i];
    }
  }
  out[in_s] = '\0';

  return out;
}
int ui_main_main();

int loan_menu(const int cur) {
	if(cur <= 0 || cur > db_members_index){
		printf("%d is not a valid loan idx\n",cur);
		return -2;
	}
  member member_cur = db_members[cur];

  char *loans[member_cur.loan.loan_index];
  char *desc[member_cur.loan.loan_index];
	

  for (int i = 0; i < member_cur.loan.loan_index; i++) {
    loan loan_cur = db_loans[member_cur.loan.loan_ids[i]];
    loans[i] = db_books[loan_cur.bookid].title;
    desc[i] = malloc(COL_SIZE_2 * sizeof(char));

    sprintf(desc[i], "Costs: $%d must return by %d/%d/%d", loan_cur.amount,
            loan_cur.return_date.day, loan_cur.return_date.month,
            loan_cur.return_date.year);
  }

  int ret = ui_menu((const char **)loans, member_cur.loan.loan_index,
                    (const char **)desc, "Loans");

  for (int i = 0; i < member_cur.loan.loan_index; i++)
    free(desc[i]);

  return ret;
}

void full_report() {
  ui_print(
      "On the following menu press enter on any item on the form to continue "
      "to the next person, press 'back' to return to the main menu");
  ui_print("press any key to continue\n");
  achar();
  printf("Members:\n");
  for (int i = 0; i < db_members_index; i++) {
    if (print_member_data(i) == -2)
      return;
  }

  printf("Books:\n");

  for (int i = 0; i < db_books_index; i++) {
    if (print_book_data(i) == -2)
      return;
  }

  return;
}

int book_menu() {
  char *books[db_books_index];
  char *desc[db_books_index];
  for (int i = 0; i < db_books_index; i++) {
    book *loc = &db_books[i];
    books[i] = loc->title;

    desc[i] = malloc(COL_SIZE_2 * sizeof(char));
    sprintf(desc[i], "ISBN: %s", loc->ISBN);
  }

  int ret = ui_menu((const char **)books, db_books_index, (const char **)desc,
                    "Books:");

  for (int i = 0; i < db_books_index; i++) {
    free(desc[i]);
  }

  return ret;
}

int member_menu() {
  char *members[db_members_index];
  char *desc[db_members_index];

  for (int i = 0; i < db_members_index; i++) {
    members[i] = malloc(COL_SIZE * sizeof(char));
    desc[i] = malloc(COL_SIZE_2 * sizeof(char));

    member *loc = &db_members[i];
    sprintf(members[i], "%s %s", loc->first_name, loc->last_name);
    sprintf(desc[i], "Has %02d loans, phone: %s", loc->loan.loan_index,
            (strlen(loc->phone_number) > 0) ? loc->phone_number
                                            : "No Phone Number");
  }

  int ret = ui_menu((const char **)members, db_members_index,
                    (const char **)desc, "Members:");

  for (int i = 0; i < db_members_index; i++) {
    free(members[i]);
    free(desc[i]);
  }

  return ret;
}

void books_genre_sort() {
  typedef struct {
    const char *genre;
    int count;
  } column_sort;

  int index = 0;
  int cap = 2;
  column_sort *genres = (column_sort *)malloc(cap * sizeof(column_sort));

  // for each book
  for (int i = 0; i < db_books_index; i++) {
    book cur_book = db_books[i];
    int found = -1;

    // for each genres
    for (int ii = 0; ii < index; ii++)
      if (strcmp(genres[ii].genre, cur_book.genre) == 0)
        found = ii;

    if (found == -1) {
      if (index == cap) {
        cap *= 2;
        column_sort *temp = realloc(genres, cap * sizeof(column_sort));

        if (!temp) {
          printf("Error couldent allocate memory for column search\n");
          return;
        }

        genres = temp;
      }

      genres[index].genre = strdup(cur_book.genre);
      genres[index].count = 1;
      index++;
    } else {
      genres[found].count++;
    }
  }

  printf("Books by genre:\n");

  char *genre_dat[index]; // base stats
  char *genre_dec[index]; // description

  for (int i = 0; i < index; i++) {
    printf("doing this..\n");
    genre_dat[i] = malloc(COL_SIZE * sizeof(char));
    genre_dec[i] = malloc(COL_SIZE_2 * sizeof(char));

    sprintf(genre_dat[i], "%s: %02d", genres[i].genre, genres[i].count);
    sprintf(genre_dec[i], "which is %0.2f%% (%d total)",
            (genres[i].count / (float)db_books_index * 100), db_books_index);
  }

  ui_menu((const char **)genre_dat, index, (const char **)genre_dec,
          "View books, Press enter to return to menu");

  for (int i = 0; i < index; i++) {
    free(genre_dat[i]);
    free(genre_dec[i]);
    free((char *)genres[i].genre);
  }

  free(genres);

  return;
}

int member_selector_menu() {
  const char *return_menu[] = {"Search by name", "Show members list",
                               "Live search"};

  const char *return_desc[] = {"", "", ""};
  int ret = ui_menu((const char **)return_menu, 2, (const char **)return_desc,
                    "Select Option");
  if (ret == -2)
    return ret;
  int sel_idx = -1;

  char buffer[100];
  if (ret == 0) {
    int sel_idx = 0;
    input(buffer, 100, "Enter your LAST name: ");
    char *l_buffer = lower(buffer);

    for (int i = 0; i < db_members_index; i++) {
      char *l_name = lower(db_members[i].last_name);
      if (strcmp(l_name, l_buffer) == 0) {
        sel_idx = i;
      }
      free(l_name);
			return sel_idx;
    }

  } else if (ret == 1) {
    // SHOW MEMBER LIST

    sel_idx = member_menu();
    if (sel_idx == -2)
      return sel_idx;
  }
  
	if (sel_idx == -1) {
      printf("last name \"%s\" not found\n", buffer);
      achar();
      return -2;
  } else {
		printf("last name \"%s\" found at index %d\n",buffer,sel_idx);
	}
  
	return sel_idx;
}
// data setup
int database() {
  // =====================================================
  // CREATE AUTHORS
  // =====================================================
  const int tolkien =
      member_add((member){.first_name = "J.R.R.",
                          .last_name = "Tolkien",
                          .type = AUTHOR,
                          .dob = (date){.year = 1892, .month = 1, .day = 3},
                          .o.author.genre = "Fantasy",
                          .o.author.is_alive = false,
                          .o.author.dod = (date){
                              .year = 1971,
                              .month = 9,
                              .day = 2,
                          }});

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
  const int alice_smith = member_add((member){.first_name = "Alice",
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

  const int bob_johnson = member_add((member){.first_name = "Bob",
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

  const int henry_davis = member_add((member){.first_name = "Henry",
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

  const int member_4 = member_add((member){.type = STAFF,
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
  loan_new(
      id_to_member_ptr(frank_lee),
      (loan){
          .bookid = old_man_and_sea,
          .issued = date_now(),
          .return_date =
              (date){
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

  loan_new(id_to_member_ptr(frank_lee), (loan){
                                            .bookid = the_hobbit,
                                            .issued = date_now(),
                                            .return_date =
                                                (date){
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
  sleep(1);
  int ret = 0;
  while (ret != -1) {
    ret = ui_main_main();
    if (ret != -1) {
      printf("Press any key to return to main menu");
      achar();
    }
  }

  printf("writing database, this may take a few seconds");
  return -12;
}

int ui_main_main() {
  int ret;
  const char *main_menu[] = {"Full report",          "Individual report",
                             "Count books by genre", "Return a book",
                             "Run a form",           "Exit",
                             "Issue a book"};

  const char *main_menu_desc[] = {
      "Print all data held in the database",
      "Select an individual user or book to view info on",
      "Show the statistical book genre counts",
      "",
      "Run a form to enter data to database",
      "Exit program",
      "Create a loan"};

  const char *list_menu[] = {
      "View members",
      "View books",
  };

  const char *form_menu[] = {
      "member",
      "book",
  };

  const char *form_desc[] = {
      "Create a member",
      "Create a book",
  };

  const char *return_menu[] = {"Search by name", "Show members list",
                               "Live search"};

  const char *return_desc[] = {"", "", ""};

  const char *list_menu_desc[] = {"", ""};
  printf("Generated book list");

  ret = ui_m(main_menu, main_menu_desc, "Select Option");

  if (ret == 0) {
    // PRINT FULL REPORT:
    full_report();

  } else if (ret == 1) {
    // INDIVIDUAL VIEW SCREEN
    ret = ui_m(list_menu, list_menu_desc, "What database do you want to view?");
    if (ret != -2) {
      if (ret == 0) {
        // INDIVIDUAL MEMBER

        ret = member_menu();
        if (ret != -2) {
          print_member_data(ret);
          for (int i = 0; i < db_members[ret].loan.loan_index; i++) {
            print_loan_data(db_loans[db_members[ret].loan.loan_ids[i]]);
          }
        }

      } else {
        // INDIVIDUAL BOOKS

        ret = book_menu();
        if (ret != -2) {
          print_book_data(ret);
        }
      }
    }
  } else if (ret == 2) {
    // SHOW BOOKS BY GENRE
    books_genre_sort();

  } else if (ret == 3) {
    // RETURN BOOK DIALOGS
    int sel_idx = member_selector_menu();
    if (sel_idx != -2) {
      member *member_cur = &db_members[sel_idx];
      if (member_cur->loan.loan_index == 0) {
        printf("selected member has no loans to return\n");
      } else {
        ret = loan_menu(sel_idx);

        if (ret != -2) {
          db_loans[member_cur->loan.loan_ids[ret]].returned = date_now();
          remove_element(member_cur->loan.loan_ids, ret,
    	                     member_cur->loan.loan_index);
          member_cur->loan.loan_index--;

          printf("Loan has been returned\n");
        }
      }
    }
  } else if (ret == 4) {
    // RUN FORMS
    ret = ui_m(form_menu, form_desc, "What form do you want to run?");
    if (ret == 0) {
      member cur = member_wizard();
			if(cur.failed_form == -1){
				printf("error occured\n");
			} else member_add(cur);
    } else if (ret == 1) {
			
      book cur = book_wizard();
			if(cur.available == -1){
				printf("error occured\n");
			} else book_add(cur);
    }

  } else if (ret == 5) {
    // EXIT UI CODE
    return -1;
  } else if (ret == 6) {
    int bookid = book_menu();
    int memberid = member_selector_menu();
    ui_print(
        "please pick the date this loan is due..\npress any key to continue\n");
    achar();
		date due = date_wizard();
		if(due.year != -1){
    loan_new(id_to_member_ptr(memberid), (loan){
                                             .bookid = bookid,
                                             .issued = date_now(),
                                             .return_date = due,
                                             .active = true,
                                             .note = "",
                                             .amount = 0, // owes nothing
                                         });
		printf("loan has been added!\n");
		} else {
			printf("error occured please try again\n");
		}
  } else if (ret == -2) {
    ui_print("There is nothing to go back too..\n");
  }
  return 12;
}
