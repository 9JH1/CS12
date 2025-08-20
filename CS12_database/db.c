#include "lib/data.h"
#include "lib/lib.h"
#include <stdio.h>
#include <string.h>

void print_datetime(date a) {
  printf("%d:%d:%d, %d/%d/%d\n", a.hour, a.minute, a.second, a.day, a.month,
         a.year);
}

/*
this is the code that is run when the database is imported
any added data is saved to the local storage when this function
exits */
int database() {
  int bookid = book_add((book){
      .title = "test",
  });

	member newMember = {
    .first_name = strdup("Alice"),
    .last_name = strdup("Johnson"),
    .email = strdup("alice.johnson@example.com"),
    .phone_number = strdup("+1234567890"),
    .dob = { .day = 15, .month = 5, .year = 1990 },
    .time_created = { .day = 20, .month = 8, .year = 2025 },
    .type = STAFF,
	};

	member_add(newMember);

  return 12;
}
