/* the database works as follows, there are several arrays that hold each type of data,
 * db_loans 
 * db_members 
 * db_books 
 *
 * each slice's type is viewable in the data.h file.
 *
 * How to symbolic links work. data in the database is linked using 
 * indexs, for example if we have zero items in db_loans and we add 
 * a new member to the fist item in the array (index of 0) then that 
 * new member has an id of 0. This system is used in all of the links 
 * for example 
 * one to one: 
 * loan bookid => book index 
 * loan member => member index 
 *
 * one to many 
 * author books => array of book indexs 
 * member loans => array of loan indexs 
 * 
 * and so on. this is why if you run a form wizard it will print out 
 * what index the new object has been made at. */


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
loan loan_wizard(void);
book book_wizard(void);

// return a pointer to a member using an memberid.
member *id_to_member_ptr(const int a);

// get the name of a member "<first_name> <last_name>"
char *member_name(member a);

// copy a member (non pointer) from an id
member id_to_member(const int a);

// print data of types: 
void print_member_data(member member_cur, int const ret);
void print_book_data(book book_cur);
void print_loan_data(loan loan_cur);

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
		#define DELETE_COMMAND "del /q"  // Drop /s unless recursive needed
		#define MKDIR_COMMAND "mkdir"
		#define MKFILE_COMMAND "type nul >"  // Truly empty file
		#define PLATFORM_NOT_SUPPORTED 0
	#else 
		#define PLATFORM_NOT_SUPPORTED 1
	#endif
	
	int dir_exist(const char *path);
	int file_exist(const char *path); 
	int achar(void);
#endif // PLATFORM_H
