#include "lib.h"
#include "../data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void input(char *buffer, int size, char *prompt) {
  printf("%s", prompt);
  if (fgets(buffer, size, stdin) != NULL) {
    // Remove newline character if present
    int len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
      buffer[len - 1] = '\0';
    }
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
  if (len > 0) {
    fwrite(str, sizeof(char), len, fp);
  }
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
    fwrite(&arr[i].dateid_issued, sizeof(int), 1, fp);
    fwrite(&arr[i].dateid_closed, sizeof(int), 1, fp);
    fwrite(&arr[i].active, sizeof(bool), 1, fp);
    write_string(fp, arr[i].note);
    fwrite(&arr[i].is.payed, sizeof(bool), 1,
           fp); // use either payed or covered since union
  }
}

loan *read_loans(FILE *fp, int *out_count) {
  fread(out_count, sizeof(int), 1, fp);
  loan *arr = malloc(*out_count * sizeof(loan));
  for (int i = 0; i < *out_count; i++) {
    fread(&arr[i].amount, sizeof(int), 1, fp);
    fread(&arr[i].dateid_issued, sizeof(int), 1, fp);
    fread(&arr[i].dateid_closed, sizeof(int), 1, fp);
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

void write_members(FILE *fp, Member *arr, int count) {
  fwrite(&count, sizeof(int), 1, fp);
  for (int i = 0; i < count; i++) {
    fwrite(arr[i].first_name, CHAR_SMALL, 1, fp);
    fwrite(arr[i].last_namer, CHAR_SMALL, 1, fp);
    fwrite(arr[i].email, CHAR_SMALL, 1, fp);
    fwrite(arr[i].phone_number, CHAR_SMALL, 1, fp);
    fwrite(&arr[i].dateid_dob, sizeof(int), 1, fp);
    fwrite(&arr[i].dateid_time_created, sizeof(int), 1, fp);
    fwrite(&arr[i].type, sizeof(MemberType), 1, fp);

    switch (arr[i].type) {
    case STAFF:
      fwrite(&arr[i].o.staff.is_hired, sizeof(bool), 1, fp);
      fwrite(&arr[i].o.staff.member_id, sizeof(int), 1, fp);
      fwrite(&arr[i].o.staff.member_code, sizeof(int), 1, fp);
      break;

    case AUTHOR:
      fwrite(arr[i].o.author.genre, CHAR_SMALL, 1, fp);
      fwrite(&arr[i].o.author.id_dod, sizeof(int), 1, fp);
      fwrite(&arr[i].o.author.is_alive, sizeof(bool), 1, fp);
      fwrite(&arr[i].o.author.book_count, sizeof(int), 1, fp);
      break;

    default:
      // For MEMBER or unknown types, just skip union data
      break;
    }

    // Write loan struct always
    fwrite(&arr[i].loan.loan_flagged, sizeof(bool), 1, fp);
    fwrite(arr[i].loan.loan_ids, sizeof(int), MAX_LOANS, fp);
  }
}

Member *read_members(FILE *fp, int *out_count) {
  fread(out_count, sizeof(int), 1, fp);
  Member *arr = malloc(*out_count * sizeof(Member));
  for (int i = 0; i < *out_count; i++) {
    fread(arr[i].first_name, CHAR_SMALL, 1, fp);
    fread(arr[i].last_namer, CHAR_SMALL, 1, fp);
    fread(arr[i].email, CHAR_SMALL, 1, fp);
    fread(arr[i].phone_number, CHAR_SMALL, 1, fp);
    fread(&arr[i].dateid_dob, sizeof(int), 1, fp);
    fread(&arr[i].dateid_time_created, sizeof(int), 1, fp);
    fread(&arr[i].type, sizeof(MemberType), 1, fp);

    switch (arr[i].type) {
    case STAFF:
      fread(&arr[i].o.staff.is_hired, sizeof(bool), 1, fp);
      fread(&arr[i].o.staff.member_id, sizeof(int), 1, fp);
      fread(&arr[i].o.staff.member_code, sizeof(int), 1, fp);
      break;

    case AUTHOR:
      fread(arr[i].o.author.genre, CHAR_SMALL, 1, fp);
      fread(&arr[i].o.author.id_dod, sizeof(int), 1, fp);
      fread(&arr[i].o.author.is_alive, sizeof(bool), 1, fp);
      fread(&arr[i].o.author.book_count, sizeof(int), 1, fp);
      break;

    default:
      // Zero union for MEMBER or unknown types
      memset(&arr[i].o, 0, sizeof(arr[i].o));
      break;
    }

    fread(&arr[i].loan.loan_flagged, sizeof(bool), 1, fp);
    fread(arr[i].loan.loan_ids, sizeof(int), MAX_LOANS, fp);
  }
  return arr;
}

void write_dates(FILE *fp, date * dates, int count){
	fwrite(&count, sizeof(int),1,fp);
	for(int i = 0;i<count;i++){
		fwrite(&dates[i].year,sizeof(int),1,fp);
		fwrite(&dates[i].month,sizeof(int),1,fp);
		fwrite(&dates[i].day,sizeof(int),1,fp);
		fwrite(&dates[i].hour, sizeof(int),1,fp);
		fwrite(&dates[i].minute,sizeof(int),1,fp);
		fwrite(&dates[i].second,sizeof(int),1,fp);
	}
}

date *read_dates(FILE *fp, int *out_count){
	fread(out_count,sizeof(int),1,fp);
	date *out = malloc(*out_count * sizeof(date));
	for(int i=0;i<*out_count;i++){
		fread(&out[i].year,sizeof(int),1,fp);
		fread(&out[i].month,sizeof(int),1,fp);
		fread(&out[i].day,sizeof(int),1,fp);
		fread(&out[i].hour,sizeof(int),1,fp);
		fread(&out[i].minute,sizeof(int),1,fp);
		fread(&out[i].second,sizeof(int),1,fp);
	}
	return out;
}
