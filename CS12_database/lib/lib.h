#ifndef DATABASE_LIB 
#define DATABASE_LIB
#include "data.h"
#include <stdio.h>

#define SECURITY_CODE_LENGTH 3
#define DATA_DIR "./db/"
#define DATA_LOANS_FNAME "loans.bin"
#define DATA_BOOKS_FNAME "books.bin"
#define DATA_MEMBERS_FNAME "members.bin"

extern loan *db_loans;
extern int db_loans_index; 
extern int db_loans_capacity;

extern member *db_members;
extern int db_members_index;
extern int db_members_capacity;

extern book *db_books;
extern int db_books_index;
extern int db_books_capacity;
date date_now(void);
char random_char(void);
void input(char *buffer, int size, char* prompt);
void write_loans(FILE *fp, loan *arr, int count);
void write_books(FILE *fp, book *arr, int count);
void members_write(FILE *a, member *arr,const int size);
int dinit_db(void);
int init_db(void);
char *combine_with_space(const char *a, const char *b); 
char *combine(const char *a, const char *b);
int book_add(const book a);
int member_add(const member a);
int loan_add(const loan a);
loan *read_loans(FILE *fp, int *out_count);
book *read_books(FILE *fp, int *out_count); 
member *read_members(FILE *a, int *out_size);
void free_member(member *m);
void print_datetime(date a);
void loan_new(member *a, loan b);
int total_loan(member a);
void print_number(const int a);
#endif // !DATABASE_LIB 
#ifndef PLATFORM_H
	// platform selection: 
	#ifdef __unix__
		// if on unix: 
		#define DELETE_COMMAND "rm -rf"
		#define MKDIR_COMMAND "mkdir"
		#define MKFILE_COMMAND "touch"
	#elif _WIN32
		// if on windows
		#define DELETE_DOMMAND "del /s /q"
		#define MKDIR_COMMAND "mkdir" // or md
		#define MKFILE_COMMAND "echo '' >"
	#else 
		#define PLATFORM_NOT_SUPPORTED 
	#endif
	
	int dir_exist(const char *path);
	int file_exist(const char *path); 
#endif // PLATFORM_H
