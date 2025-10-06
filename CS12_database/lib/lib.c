#include "data.h"
#include "lib.h"
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
    fwrite(arr[i].note, CHAR_LARGE, 1, fp);  // Fixed: Write full fixed-size array
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
    fread(arr[i].note, CHAR_LARGE,1,fp);  // Fixed: Read full fixed-size array, remove &
    arr[i].note[CHAR_LARGE - 1] = '\0';   // Fixed: Ensure null-termination
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

  // Count non-deleted members
  int effective_size = 0;
  for (int i = 0; i < size; ++i) {
    if (!arr[i].account_to_delete) effective_size++;
  }

  // Write the effective size
  fwrite(&effective_size, sizeof(int), 1, a);

  // Write non-deleted members
  for (int i = 0; i < size; ++i) {
    const member *m = &arr[i];
    if (m->account_to_delete) continue;

    fwrite(m->first_name, CHAR_SMALL, 1, a);
    fwrite(m->last_name, CHAR_SMALL, 1, a);
    fwrite(m->email, CHAR_SMALL, 1, a);
    fwrite(m->phone_number, CHAR_SMALL, 1, a);

    fwrite(&m->dob, sizeof(date), 1, a);
    fwrite(&m->time_created, sizeof(date), 1, a);
    fwrite(&m->type, sizeof(memberType), 1, a);
    fwrite(&m->account_available, sizeof(bool), 1, a);

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
      fwrite(m->o.author.genre, CHAR_SMALL, 1, a);
      fwrite(&m->o.author.dod, sizeof(date), 1, a);
      fwrite(&m->o.author.is_alive, sizeof(bool), 1, a);
    }
  }
}

member *read_members(FILE *a, int *out_size) {
    FILE *fp = (FILE *)a;
    int size = 0;
    fread(&size, sizeof(int), 1,fp);
    if(size == 0){
		*out_size = 0;
		return malloc(sizeof(member));
	}

  member *arr = calloc(size, sizeof(member));
  for (int i = 0; i < size; ++i) {
    member *m = &arr[i];

		fread(m->first_name,CHAR_SMALL,1,fp);
		fread(m->last_name,CHAR_SMALL,1,fp);
		fread(m->email,CHAR_SMALL,1,fp);
		fread(m->phone_number,CHAR_SMALL,1,fp);

    fread(&m->dob, sizeof(date), 1, fp);
    fread(&m->time_created, sizeof(date), 1, fp);
    fread(&m->type, sizeof(memberType), 1, fp);

    // Loan
    fread(&m->loan.loan_flagged, sizeof(bool), 1, fp);
    fread(&m->loan.loan_index, sizeof(int), 1, fp);
    fread(&m->loan.loan_capacity, sizeof(int), 1, fp);

    if (m->loan.loan_capacity > 0) {
      m->loan.loan_ids = malloc(m->loan.loan_capacity * sizeof(int));
      fread(m->loan.loan_ids, sizeof(int), m->loan.loan_capacity, fp);
    } else m->loan.loan_ids = NULL;

    // Union
    if (m->type == STAFF) {
      fread(&m->o.staff.is_hired, sizeof(bool), 1, fp);
      fread(&m->o.staff.member_id, sizeof(int), 1, fp);
      fread(&m->o.staff.member_code, sizeof(int), 1, fp);
    } else if (m->type == AUTHOR) {
      fread(m->o.author.genre,CHAR_SMALL,1,fp);  // Fixed: Remove &
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
    error_flag = true;
  } else {
    db_loans = read_loans(loans_path, &db_loans_index);
		if(db_loans == NULL || db_loans_index == 0){
			printf("db_loans dident load any data\ninitializing now...\n");
			db_loans_capacity = DEFAULT_DB_CAPACITY;
			db_loans = malloc(db_loans_capacity * sizeof(loan));
			if(db_loans == NULL){
				printf("db_loans could not be initialized\n");
				return -5;
			}
		} else db_loans_capacity = (db_loans_index + 1) * 2;
		printf("Imported %d Loans (capacity: %d) from %s to db_loans\n", db_loans_index, db_loans_capacity,DATA_LOANS_FNAME);
  }

  if (books_path == NULL) {
    printf("Couldent open \"%s\"\n", books_path_char);
    char *com = combine_with_space(MKFILE_COMMAND, books_path_char);
		system(com);
		free(com);
    printf("Created \"%s\" file\n", books_path_char);
    error_flag = true;
  } else {
    db_books = read_books(books_path, &db_books_index);
		if (db_books == NULL || db_books_index == 0){
			printf("db_books dident load any data\ninitializing now...\n");
			db_books_capacity = DEFAULT_DB_CAPACITY;
			db_books = malloc(db_books_capacity * sizeof(book));
			if(db_books == NULL){
				printf("db_books could not be initialized\n");
				return -5;
			}
		} else db_books_capacity = (db_books_index + 1) * 2;	
		printf("Imported %d Books (capacity: %d) from %s to db_loans\n", db_books_index, db_books_capacity,DATA_BOOKS_FNAME);
  }

  if (members_path == NULL) {
    printf("Couldent open \"%s\"\n", members_path_char);
    char *com = combine_with_space(MKFILE_COMMAND, members_path_char);
		system(com);
		free(com);
    printf("Created \"%s\" file\n", members_path_char);
    error_flag = true;
  } else {
    db_members = read_members(members_path, &db_members_index);
		if(db_members == NULL || db_members_index == 0){
			printf("db_members dident load any data\ninitializing now...\n");
			db_members_capacity = DEFAULT_DB_CAPACITY;
			db_members = malloc(db_members_capacity * sizeof(member));
			if(db_members == NULL){
				printf("db_members could not be initialized\n");
				return -5;
			}
		} else db_members_capacity = (db_members_index + 1) * 2;
  	printf("Imported %d members (capacity: %d) from %s to db_members\n", db_members_index, db_members_capacity, DATA_MEMBERS_FNAME);
  }

  free(loans_path_char);
  free(books_path_char);
  free(members_path_char);

  if (error_flag == true) return 1;

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
        member *temp = (member *)realloc(db_members, db_members_capacity * sizeof(member));

        if (!temp) {
            printf("Error allocating memory for members\n");
            return -1;
        }

        db_members = temp;
    }

    db_members[db_members_index] = a;
    if (a.loan.loan_capacity > 0 && a.loan.loan_ids != NULL) {
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


void print_book_data(book book_cur) {
  printf("\nBook Metadata:\n");
  printf("title: %s\n", book_cur.title);
  printf("id_author: %d (id linking to the author of the book)\n",
         book_cur.id_author);
  printf("ISBN: %s\n", book_cur.ISBN);
  printf("genre: %s\n", book_cur.genre);
  printf("publication_date: %2d/%2d/%d, %2d:%2d:%2d\n",
         book_cur.publication_date.day, book_cur.publication_date.month,
         book_cur.publication_date.year, book_cur.publication_date.hour,
         book_cur.publication_date.minute, book_cur.publication_date.second);

  printf("available: %d (how many of this book are currently on loan)\n",
         book_cur.available);
  printf("count: %d (how many of this book there is in total)\n",
         book_cur.count);
  printf("-----------\n");
  printf("Symbolic Metadata:\n");

  member author = db_members[book_cur.id_author];
  printf("Author Name: %s %s\n", author.first_name, author.last_name);
}

void print_loan_data(loan loan_cur){
	printf("amount (owed): $%d\n",loan_cur.amount);
	printf("issued: %d/%d/%d - %d:%d:%d\n",
			loan_cur.issued.day,
			loan_cur.issued.month,
			loan_cur.issued.year,
			loan_cur.issued.hour,
			loan_cur.issued.minute,
			loan_cur.issued.second
	);
	printf("return_date (due date): %d/%d/%d - %d:%d:%d\n",
			loan_cur.return_date.day,
			loan_cur.return_date.month,
			loan_cur.return_date.year,
			loan_cur.return_date.hour,
			loan_cur.return_date.minute,
			loan_cur.return_date.second);

	printf("returned (when user returned book): %d/%d/%d - %d:%d:%d\n",
			loan_cur.returned.day,
			loan_cur.returned.month,
			loan_cur.returned.year,
			loan_cur.returned.hour,
			loan_cur.returned.minute,
			loan_cur.returned.second);

	printf("bookid %d\n",loan_cur.bookid);
	printf("active (is the loan currently not returned) %d ( 0 = false 1 = true)\n ",loan_cur.active);
	printf("is.payed (has the loan be payed) %d\n", loan_cur.is.payed);
	printf("is.covered (has the loan been payed) %d\n", loan_cur.is.covered);
	printf("note: %s\n",loan_cur.note);
	printf("SYMBOLIC DATA ===============\n");
	printf("Book information:\n");
	print_book_data(db_books[loan_cur.bookid]);
}

void print_member_data(member member_cur,const int ret) {
  printf("\nMember Metadata:\n");
  printf("first_name: %s\n", member_cur.first_name);
  printf("last_name: %s\n", member_cur.last_name);
  printf("DOB: %0d/%0d/%d, %0d:%0d:%0d\n", member_cur.dob.day,
         member_cur.dob.month, member_cur.dob.year, member_cur.dob.hour,
         member_cur.dob.minute, member_cur.dob.second);

  printf("email: %s\n", member_cur.email);
  printf("phone_number: %s\n", member_cur.phone_number);
  printf("account_available: %d (0 = false, 1 = true)\n",
         member_cur.account_available);
  printf("account_to_delete: %d (0 = false, 1 = true)\n",
         member_cur.account_to_delete);
  if (member_cur.type == STAFF) {
    printf("type: STAFF\n");
    printf("o.staff.member_code: %d\n", member_cur.o.staff.member_code);
    printf("o.staff.staff_id: %d\n", member_cur.o.staff.member_id);
    printf("o.staff.is_hired: %d (0 = false, 1 = true)\n",
           member_cur.o.staff.is_hired);
  } else if (member_cur.type == AUTHOR) {
    printf("type: AUTHOR\n");
    printf("o.author.genre: %s\n", member_cur.o.author.genre);
    if (member_cur.o.author.is_alive) {
      printf("Author is alive\n");
    } else
      printf("Author died %d/%d/%d, %d:%d:%d\n", member_cur.o.author.dod.day,
             member_cur.o.author.dod.month, member_cur.o.author.dod.year,
             member_cur.o.author.dod.hour, member_cur.o.author.dod.minute,
             member_cur.o.author.dod.second);
  } else
    printf("type: MEMBER\n");
  printf("------------\n");
  printf("Symbolic metadata\n");

  if (member_cur.type == AUTHOR) {
    int count = 0;
    for (int i = 0; i < db_books_index; i++)
      if (db_books[i].id_author == ret)
        count++;
    printf("Author Book Count: %d\n", count);
  } else
    printf("NA\n");
}
