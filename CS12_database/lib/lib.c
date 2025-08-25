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
    arr[i].note = read_string(fp);
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


// Helper macro to write strings safely
#define WRITE_STRING(s)                                                        \
  do {                                                                         \
    int len = (s != NULL) ? (int)strlen(s) + 1 : 0;                            \
    fwrite(&len, sizeof(int), 1, a);                                           \
    if (len > 0) {                                                             \
      fwrite(s, sizeof(char), len, a);                                         \
    }                                                                          \
  } while (0)

// Read strings with length
#define READ_STRING(s)                                                         \
  do {                                                                         \
    int len;                                                                   \
    fread(&len, sizeof(int), 1, fp);                                           \
    if (len > 0) {                                                             \
      s = malloc(len);                                                         \
      fread(s, sizeof(char), len, fp);                                         \
    } else {                                                                   \
      s = NULL;                                                                \
    }                                                                          \
  } while (0)

void write_members(FILE *a, member *arr, int size) {
  if (!a || !arr || size < 0) return;
	for(int i = 0;i < size; ++i) if(db_members[i].account_to_delete == true) size--;
  fwrite(&size, sizeof(int), 1, a);

  for (int i = 0; i < size; ++i) {
    const member *m = &arr[i];
		if(m->account_to_delete) continue;

    WRITE_STRING(m->first_name);
    WRITE_STRING(m->last_name);
    WRITE_STRING(m->email);
    WRITE_STRING(m->phone_number);

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
      WRITE_STRING(m->o.author.genre);
      fwrite(&m->o.author.dod, sizeof(date), 1, a);
      fwrite(&m->o.author.is_alive, sizeof(bool), 1, a);
    }

  }
}

void free_member(member *m) {
  if (!m) return;
	if(m->first_name) free(m->first_name);
  if(m->last_name) free(m->last_name);
  if(m->email) free(m->email);
  if(m->phone_number) free(m->phone_number);
  if (m->loan.loan_ids) free(m->loan.loan_ids);
  if (m->type == AUTHOR) free(m->o.author.genre);
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
    READ_STRING(m->first_name);
    READ_STRING(m->last_name);
    READ_STRING(m->email);
    READ_STRING(m->phone_number);

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
      READ_STRING(m->o.author.genre);
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
    db_loans_capacity = (db_loans_index + 1) * 2;
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
    db_books_capacity = (db_books_index + 1) * 2;
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
    db_members_capacity = (db_members_index + 1) * 2;
  }

  free(loans_path_char);
  free(books_path_char);
  free(members_path_char);

  if (error_flag == true) return 1;

  printf("Imported %d Loans (capacity: %d)\n", db_loans_index, db_loans_capacity);
  printf("Imported %d Books (capacity: %d)\n", db_books_index, db_books_capacity);
  printf("Imported %d members (capacity: %d)\n", db_members_index, db_members_capacity);
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

int member_add(const member a) {
  if (db_members_index == db_members_capacity) {
    db_members_capacity *= 2;
    member *temp = realloc(db_members, db_members_index * sizeof(member));

    if (!temp) {
      printf("Error allocating memory for members\n");
      return -1;
    }

    db_members = temp;
  }
	
	if(db_members == NULL){
		printf("Error db_members is unininitialized\n");
		return -1;
	}

  db_members[db_members_index] = a;
  db_members_index++;
	printf("created new member at index %d\n",db_members_index-1);
  return db_members_index - 1;
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
