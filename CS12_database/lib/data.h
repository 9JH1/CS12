#ifndef DATA_TYPES_H
#define DATA_TYPES_H
#include <stdbool.h> // for (true|false) and boolian type.

#define CHAR_LARGE 255
#define CHAR_SMALL 127
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
	char *note;       // note of loan
  union {
    bool payed;
    bool covered;
  } is;
} loan;

// covers members staff and authors
typedef struct {
	// contact info: 
  char *first_name;
  char *last_name;
  char *email;
  char *phone_number;

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
      char *genre;
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
