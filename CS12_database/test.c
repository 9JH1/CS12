#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Assume all your types, constants, and read/write functions are declared here
// For example, loan, Member, book, write_loans(), read_loans(), etc.

#define CHAR_SMALL 32
#define CHAR_LARGE 128

typedef enum { TYPE_A, TYPE_B } MemberType;

typedef struct {
    int day, month, year;  // simple example for date
} date;

// Implement write_string() and read_string() here from previous snippet

int main() {
    // --- Prepare some sample data ---

    loan loans[2];
    loans[0].amount = 1000;
    loans[0].dateid_issued = 20230801;
    loans[0].dateid_closed = 0;
    loans[0].active = true;
    loans[0].note = strdup("First loan note");
    loans[0].is.payed = true;

    loans[1].amount = 500;
    loans[1].dateid_issued = 20230715;
    loans[1].dateid_closed = 20230815;
    loans[1].active = false;
    loans[1].note = strdup("Second loan note");
    loans[1].is.payed = false;

    Member members[1];
    strcpy(members[0].first_name, "John");
    strcpy(members[0].last_namer, "Doe");
    strcpy(members[0].email, "john.doe@example.com");
    strcpy(members[0].phone_number, "1234567890");
    members[0].dateid_dob = 19850101;
    members[0].dateid_time_created = 20220101;
    members[0].type = TYPE_A;
    members[0].loan.id_loan = 1;
    members[0].loan.loan_amount = 1000;
    members[0].loan.loan_capacity = 1000;
    members[0].loan.loan_type.loan_dynamic = false;
    members[0].o.staff.is_hired = true;
    members[0].o.staff.member_id = 42;
    members[0].o.staff.member_code = 1234;

    book books[1];
    strcpy(books[0].title, "Example Book");
    books[0].id_author = 42;
    books[0].publication_date = (date){1, 1, 2023};
    strcpy(books[0].genre, "Fiction");
    strcpy(books[0].ISBN, "123-4567890123");
    books[0].available = true;

    // --- Write arrays to files ---

    FILE *f_loans = fopen("loans.dat", "wb");
    write_loans(f_loans, loans, 2);
    fclose(f_loans);

    FILE *f_members = fopen("members.dat", "wb");
    write_members(f_members, members, 1);
    fclose(f_members);

    FILE *f_books = fopen("books.dat", "wb");
    write_books(f_books, books, 1);
    fclose(f_books);

    // --- Free dynamic loan notes before reading back ---
    free(loans[0].note);
    free(loans[1].note);

    // --- Read arrays back from files ---

    int count_loans, count_members, count_books;

    f_loans = fopen("loans.dat", "rb");
    loan *loaded_loans = read_loans(f_loans, &count_loans);
    fclose(f_loans);

    f_members = fopen("members.dat", "rb");
    Member *loaded_members = read_members(f_members, &count_members);
    fclose(f_members);

    f_books = fopen("books.dat", "rb");
    book *loaded_books = read_books(f_books, &count_books);
    fclose(f_books);

    // --- Example: print loaded loans ---
    printf("Loaded %d loans:\n", count_loans);
    for (int i = 0; i < count_loans; i++) {
        printf("Loan %d: amount=%d, active=%d, note=%s, payed=%d\n",
            i, loaded_loans[i].amount, loaded_loans[i].active,
            loaded_loans[i].note ? loaded_loans[i].note : "(null)",
            loaded_loans[i].is.payed);
    }

    // Don't forget to free loaded dynamic strings!
    for (int i = 0; i < count_loans; i++) {
        free(loaded_loans[i].note);
    }
    free(loaded_loans);
    free(loaded_members);
    free(loaded_books);

    return 0;
}
