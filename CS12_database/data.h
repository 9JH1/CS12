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
} MemberType;

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
  int dateid_issued;
  int dateid_closed;
  bool active;
  char *note;
  union {
    bool payed;
    bool covered;
  } is;
} loan;

// covers members staff and authors
typedef struct {
  char first_name[CHAR_SMALL];
  char last_namer[CHAR_SMALL];
  char email[CHAR_SMALL];
  char phone_number[CHAR_SMALL];
  int dateid_dob;
  int dateid_time_created;
  MemberType type;
  struct {
    bool loan_flagged;
    int loan_ids[MAX_LOANS];
  } loan;
  union {
    struct {
      bool is_hired;
      int member_id;
      int member_code;
    } staff;
    struct {
      char genre[CHAR_SMALL];
      int id_dod;
      bool is_alive;
      int book_count;
    } author;
  } o;

} Member;

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
