#ifndef DATA_TYPES_H
#define DATA_TYPES_H
#include <stdbool.h>

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
  int amount;
  date issued;
  date closed;
  bool active;
  char *note;
  union {
    bool payed;
    bool covered;
  } is;
} loan;

// covers members staff and authors
typedef struct {
  char *first_name;
  char *last_name;
  char *email;
  char *phone_number;
  date dob;
  date time_created;
  memberType type;
  struct {
  	bool loan_flagged; 
    int *loan_ids;
		int loan_index;
		int loan_capacity;
  } loan;
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
      int book_count;
    } author;
  } o;
} member;

// covers all books
typedef struct {
  char title[CHAR_LARGE];
  int id_author;
  date publication_date;
  char genre[CHAR_SMALL];
  char ISBN[15];
  bool available;
} book;

#endif // DATA_TYPES_H
#define DATA_TYPES_H
