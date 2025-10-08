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

char *combine_with_space(const char *cmd, const char *path) {
  char *quoted_path = malloc(strlen(path) + 3);  // +2 quotes +1 nul
  if (!quoted_path) return NULL;
  sprintf(quoted_path, "\"%s\"", path);
  char *com = malloc(strlen(cmd) + 1 + strlen(quoted_path) + 1);
  if (!com) { free(quoted_path); return NULL; }
  sprintf(com, "%s %s", cmd, quoted_path);
  free(quoted_path);
  return com;
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



// Function to create a member object with user input and error checking
member member_wizard() {
    char first_name[CHAR_SMALL+1];
    char last_name[CHAR_SMALL+1];
    char email[CHAR_SMALL+1];
    char phone_number[CHAR_SMALL+1];
    char type_input[CHAR_SMALL+1];
    char genre[CHAR_SMALL+1];
    char is_hired_input[CHAR_SMALL+1];
    char is_alive_input[CHAR_SMALL+1];
    char member_id_input[CHAR_SMALL+1];
    char member_code_input[CHAR_SMALL+1];
    member new_member = {.account_available = false, .account_to_delete = false};

    // Tip for input formatting
    printf("Ensure any values of 0 are formatted like '0' not '00' or otherwise\n");

    // Input for first name
    input(first_name, CHAR_SMALL, "Enter First Name: ");
    if (strlen(first_name) == 0) {
        printf("first_name \"%s\" is invalid\n", first_name);
        return new_member;
    }

    // Input for last name
    input(last_name, CHAR_SMALL, "Enter Last Name: ");
    if (strlen(last_name) == 0) {
        printf("last_name \"%s\" is invalid\n", last_name);
        return new_member;
    }

    // Input for email
    input(email, CHAR_SMALL, "Enter Email: ");
    if (strlen(email) == 0 || strchr(email, '@') == NULL) {
        printf("email \"%s\" is invalid\n", email);
        return new_member;
    }

    // Input for phone number
    input(phone_number, CHAR_SMALL, "Enter Phone Number: ");
    if (strlen(phone_number) == 0) {
        printf("phone_number \"%s\" is invalid\n", phone_number);
        return new_member;
    }

    // Input for date of birth
    printf("Enter Date of Birth:\n");
    new_member.dob = date_wizard();
    if (new_member.dob.year == -1) {
        printf("Invalid date of birth\n");
        return new_member;
    }

    // Input for member type
    input(type_input, CHAR_SMALL, "Enter Member Type (0=Member, 1=Author, 2=Staff): ");
    int itype = eatoi(type_input);
    if (itype < 0 || itype > 2) {
        printf("member type \"%s\" is invalid\n", type_input);
        return new_member;
    }
    new_member.type = (memberType)itype;

    // Handle type-specific fields
    if (new_member.type == STAFF) {
        // Staff-specific inputs
        input(is_hired_input, CHAR_SMALL, "Is Staff Hired (0=false, 1=true): ");
        int is_hired = eatoi(is_hired_input);
        if (is_hired == -1 || is_hired < 0 || is_hired > 1) {
            printf("is_hired \"%s\" is invalid\n", is_hired_input);
            return new_member;
        }
        input(member_id_input, CHAR_SMALL, "Enter Staff Member ID: ");
        int member_id = eatoi(member_id_input);
        if (member_id == -1) {
            printf("member_id \"%s\" is invalid\n", member_id_input);
            return new_member;
        }
        input(member_code_input, CHAR_SMALL, "Enter Staff Member Code: ");
        int member_code = eatoi(member_code_input);
        if (member_code == -1) {
            printf("member_code \"%s\" is invalid\n", member_code_input);
            return new_member;
        }
        new_member.o.staff.is_hired = (bool)is_hired;
        new_member.o.staff.member_id = member_id;
        new_member.o.staff.member_code = member_code;
    } else if (new_member.type == AUTHOR) {
        // Author-specific inputs
        input(genre, CHAR_SMALL, "Enter Author Genre: ");
        if (strlen(genre) == 0) {
            printf("genre \"%s\" is invalid\n", genre);
            return new_member;
        }
        input(is_alive_input, CHAR_SMALL, "Is Author Alive (0=false, 1=true): ");
        int is_alive = eatoi(is_alive_input);
        if (is_alive == -1 || is_alive < 0 || is_alive > 1) {
            printf("is_alive \"%s\" is invalid\n", is_alive_input);
            return new_member;
        }
        printf("Enter Date of Death (if applicable, else enter invalid date):\n");
        new_member.o.author.dod = date_wizard();
        if (is_alive && new_member.o.author.dod.year != -1) {
            printf("Date of death provided but author is marked alive\n");
            return new_member;
        }
        strncpy(new_member.o.author.genre, genre, CHAR_SMALL);
        new_member.o.author.is_alive = (bool)is_alive;
    }

    // Initialize loan data
    new_member.loan.loan_flagged = false;
    new_member.loan.loan_ids = NULL;
    new_member.loan.loan_index = 0;
    new_member.loan.loan_capacity = 0;

    // Set other fields
    new_member.account_available = true;
    new_member.account_to_delete = false;
    new_member.time_created = date_wizard(); // Assuming current time for creation
    if (new_member.time_created.year == -1) {
        printf("Invalid creation date\n");
        return new_member;
    }

    // Copy strings to member struct
    strncpy(new_member.first_name, first_name, CHAR_SMALL);
    strncpy(new_member.last_name, last_name, CHAR_SMALL);
    strncpy(new_member.email, email, CHAR_SMALL);
    strncpy(new_member.phone_number, phone_number, CHAR_SMALL);

    return new_member;
}

// Function to create a loan object with user input and error checking
loan loan_wizard() {
    char amount_input[CHAR_SMALL+1];
    char bookid_input[CHAR_SMALL+1];
    char active_input[CHAR_SMALL+1];
    char payed_input[CHAR_SMALL+1];
    char note[CHAR_LARGE+1];
    loan new_loan = {.active = false, .is.payed = false};

    // Tip for input formatting
    printf("Ensure any values of 0 are formatted like '0' not '00' or otherwise\n");

    // Input for amount
    input(amount_input, CHAR_SMALL, "Enter Loan Amount: ");
    int iamount = eatoi(amount_input);
    if (iamount == -1 || iamount < 0) {
        printf("amount \"%s\" is invalid\n", amount_input);
        return new_loan;
    }

    // Input for book ID
    input(bookid_input, CHAR_SMALL, "Enter Book ID: ");
    int ibookid = eatoi(bookid_input);
    if (ibookid == -1) {
        printf("bookid \"%s\" is invalid\n", bookid_input);
        return new_loan;
    }

    // Input for issued date
    printf("Enter Issued Date:\n");
    new_loan.issued = date_wizard();
    if (new_loan.issued.year == -1) {
        printf("Invalid issued date\n");
        return new_loan;
    }

    // Input for return date
    printf("Enter Return Date:\n");
    new_loan.return_date = date_wizard();
    if (new_loan.return_date.year == -1) {
        printf("Invalid return date\n");
        return new_loan;
    }

    // Input for returned date (optional, can be invalid if not returned)
    printf("Enter Returned Date (if applicable, else enter invalid date):\n");
    new_loan.returned = date_wizard();

    // Input for active status
    input(active_input, CHAR_SMALL, "Is Loan Active (0=false, 1=true): ");
    int iactive = eatoi(active_input);
    if (iactive == -1 || iactive < 0 || iactive > 1) {
        printf("active \"%s\" is invalid\n", active_input);
        return new_loan;
    }

    // Input for payed/covered status
    input(payed_input, CHAR_SMALL, "Is Loan Payed/Covered (0=false, 1=true): ");
    int ipayed = eatoi(payed_input);
    if (ipayed == -1 || ipayed < 0 || ipayed > 1) {
        printf("payed \"%s\" is invalid\n", payed_input);
        return new_loan;
    }

    // Input for note
    input(note, CHAR_LARGE, "Enter Loan Note: ");
    if (strlen(note) == 0) {
        printf("note \"%s\" is invalid\n", note);
        return new_loan;
    }

    // Create loan object
    new_loan = (loan){
        .amount = iamount,
        .bookid = ibookid,
        .issued = new_loan.issued,
        .returned = new_loan.returned,
        .return_date = new_loan.return_date,
        .active = (bool)iactive,
        .is.payed = (bool)ipayed
    };
    strncpy(new_loan.note, note, CHAR_LARGE);

    return new_loan;
}

// Function to create a book object with user input and error checking
book book_wizard() {
    char title[CHAR_LARGE+1];
    char id_author_input[CHAR_SMALL+1];
    char genre[CHAR_SMALL+1];
    char ISBN[15+1];
    char available_input[CHAR_SMALL+1];
    char count_input[CHAR_SMALL+1];
    book new_book = {.available = -1, .count = -1};

    // Tip for input formatting
    printf("Ensure any values of 0 are formatted like '0' not '00' or otherwise\n");

    // Input for title
    input(title, CHAR_LARGE, "Enter Book Title: ");
    if (strlen(title) == 0) {
        printf("title \"%s\" is invalid\n", title);
        return new_book;
    }

    // Input for author ID
    input(id_author_input, CHAR_SMALL, "Enter Author ID: ");
    int iid_author = eatoi(id_author_input);
    if (iid_author == -1) {
        printf("author ID \"%s\" is invalid\n", id_author_input);
        return new_book;
    }

    // Input for publication date
    printf("Enter Publication Date:\n");
    new_book.publication_date = date_wizard();
    if (new_book.publication_date.year == -1) {
        printf("Invalid publication date\n");
        return new_book;
    }

    // Input for genre
    input(genre, CHAR_SMALL, "Enter Genre: ");
    if (strlen(genre) == 0) {
        printf("genre \"%s\" is invalid\n", genre);
        return new_book;
    }

    // Input for ISBN
    input(ISBN, 15, "Enter ISBN (13 digits): ");
    if (strlen(ISBN) != 13) {
        printf("ISBN \"%s\" is invalid (must be 13 digits)\n", ISBN);
        return new_book;
    }

    // Input for available count
    input(available_input, CHAR_SMALL, "Enter Number of Books Available: ");
    int iavailable = eatoi(available_input);
    if (iavailable == -1 || iavailable < 0) {
        printf("available count \"%s\" is invalid\n", available_input);
        return new_book;
    }

    // Input for total count
    input(count_input, CHAR_SMALL, "Enter Total Number of Books: ");
    int icount = eatoi(count_input);
    if (icount == -1 || icount < iavailable) {
        printf("total count \"%s\" is invalid or less than available count\n", count_input);
        return new_book;
    }

    // Create book object
    new_book = (book){
        .id_author = iid_author,
        .publication_date = new_book.publication_date,
        .available = iavailable,
        .count = icount
    };
    strncpy(new_book.title, title, CHAR_LARGE);
    strncpy(new_book.genre, genre, CHAR_SMALL);
    strncpy(new_book.ISBN, ISBN, 15);

    return new_book;
}
