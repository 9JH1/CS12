#ifndef DATABASE_LIB 
#define DATABASE_LIB
#include "../data.h"
#include <stdio.h>

char random_char(void);
void input(char *buffer, int size, char* prompt);
void write_loans(FILE *fp, loan *arr, int count);
void write_books(FILE *fp, book *arr, int count);
void write_members(FILE *fp, Member *arr, int count);
void write_dates(FILE *fp, date *dates, int count);

loan *read_loans(FILE *fp, int *out_count);
book *read_books(FILE *fp, int *out_count); 
Member *read_members(FILE *fp, int *out_count);
date *read_dates(FILE *fp, int *out_count);
#endif // !DATABASE_LIB 
