#include "lib.h"
#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

		fwrite(&m->first_name,CHAR_SMALL,1,a);
		fwrite(&m->last_name, CHAR_SMALL,1,a);
		fwrite(&m->email,CHAR_SMALL,1,a);
		fwrite(&m->phone_number,CHAR_SMALL,1,a);

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
	fread(&size, sizeof(int), 1,fp);
	if(size == 0){
		*out_size = 0;
		return malloc(sizeof(member));
	}

  member *arr = calloc(size, sizeof(member));
  for (int i = 0; i < size; ++i) {
    member *m = &arr[i];

		fread(&m->first_name,CHAR_SMALL,1,fp);
		fread(&m->last_name,CHAR_SMALL,1,fp);
		fread(&m->email,CHAR_SMALL,1,fp);
		fread(&m->phone_number,CHAR_SMALL,1,fp);

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
      fread(&m->o.author.genre,CHAR_SMALL,1,fp);
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
		if(db_loans == NULL){
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
		if (db_books == NULL){
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
		if(db_members == NULL){
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
        // Double the capacity or set a minimum to avoid frequent reallocations
        size_t new_capacity = (db_members_capacity == 0) ? 2 : db_members_capacity * 2;
        member *temp = realloc(db_members, new_capacity * sizeof(member));

        if (!temp) {
            printf("Error allocating memory for members\n");
            return -1;
        }

        db_members = temp;
        db_members_capacity = new_capacity;
    }

    db_members[db_members_index] = a;
    db_members_index++;
    printf("Created new member at index %i\n", db_members_index - 1);
    return db_members_index - 1;
}
