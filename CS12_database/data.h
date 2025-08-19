#ifndef DATA_TYPES_H
#define DATA_TYPES_H 
#include <stdbool.h> 

#define CHAR_LARGE 255 
#define CHAR_SMALL 127

typedef enum {
	REGULAR   = 0,
	VOLUNTEER = 1,
	STAFF     = 2,
	ADMIN     = 3,
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
	date date_incurred;
	date date_payed;
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
	date dob;
	date time_created;
	MemberType type;
	char *note;
	struct {
		char *note;
		loan *loan;
		int loan_amount;
		int loan_capacity; // loan_amount = loan_capacity: loan_flagged = true;
		union { 
			bool loan_dynamic; // dynamically increase capacity if not set then default to loan_flagged
			bool loan_flagged; // loan is flagged ( regular loan ); 
		} loan_type;
	} loan;
	union {
		struct {
			bool is_hired;
			int member_id;
			int member_code;
		} staff;
		struct { 
			char genre[CHAR_SMALL];
			date dod;
			bool is_alive;
			int book_count;
		} author;
	} o;
} Member;

// covers all books
typedef struct {
	char title[CHAR_LARGE];
	Member author;
	date publication_date;
	char genre[CHAR_SMALL];	
	char ISBN[15];
	bool available;
} book;


#endif //DATA_TYPES_H
#define DATA_TYPES_H 
