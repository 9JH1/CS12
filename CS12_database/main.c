#include "data.h"
#include "lib/lib.h" 
#include "lib/plib.h"
#include "platform.h"
#include "db.c"


#include <stdlib.h>
#include <stdio.h> 
#include <signal.h> 
#include <string.h> 
#include <stdbool.h>


#define SECURITY_CODE_LENGTH 10
#define DATA_DIR "./db/"
#define DATA_LOANS_FNAME "loans.bin"
#define DATA_BOOKS_FNAME "books.bin"
#define DATA_DATES_FNAME "dates.bin"
#define DATA_MEMBERS_FNAME "members.bin"


// actual local database 
loan *db_loans;
int db_loans_index; 
int db_loans_capacity;

Member *db_members;
int db_members_index;
int db_members_capacity;

book *db_books;
int db_books_index;
int db_books_capacity;

date *db_dates;
int db_dates_index;
int db_dates_capacity;

void quit(int code){
	if(code != 0)
		printf("\rexiting program with code %d..\n",code);
	dinit_argument_list();
	exit(code);
}

char *combine_with_space(const char *a, const char *b){
	const int size = snprintf(NULL,0,"%s %s",a,b);
	char *out = malloc(size + 1);
	sprintf(out,"%s %s",a,b);
	return out;
}

char *combine(const char *a, const char*b){
	const int size = snprintf(NULL,0,"%s%s",a,b);
	char *out = (char *)malloc(size +1);
	sprintf(out,"%s%s",a,b);
	return out;
}


// load the database safley
int init_db(){
	printf("Importing Database..\n");
	char *loans_path_char = combine(DATA_DIR,DATA_LOANS_FNAME);
	char *books_path_char = combine(DATA_DIR,DATA_BOOKS_FNAME);
	char *dates_path_char = combine(DATA_DIR, DATA_DATES_FNAME);
	char *members_path_char = combine(DATA_DIR, DATA_MEMBERS_FNAME);
	
	FILE *loans_path = fopen(loans_path_char, "rb");
	FILE *books_path = fopen(books_path_char, "rb");
	FILE *dates_path = fopen(dates_path_char, "rb");
	FILE *members_path = fopen(members_path_char, "rb");
	
	bool error_flag = false;

	if(loans_path == NULL){
		printf("Couldent open \"%s\"\n",loans_path_char);
		system(combine_with_space(MKFILE_COMMAND,loans_path_char));
		printf("Created \"%s\" file\n",loans_path_char);
		error_flag = true;
	} else { 
		db_loans = read_loans(loans_path, &db_loans_index);
		db_loans_capacity = db_loans_index + 1 * 2;
	}

	if(books_path == NULL){
		printf("Couldent open \"%s\"\n",books_path_char);
		system(combine_with_space(MKFILE_COMMAND,books_path_char));
		printf("Created \"%s\" file\n",books_path_char);

		error_flag = true;
	} else { 
		db_books = read_books(books_path, &db_books_index);
		db_books_capacity = db_books_index + 1 * 2;
	}
	
	if(dates_path == NULL){
		printf("Couldent open \"%s\"\n",dates_path_char);
		system(combine_with_space(MKFILE_COMMAND,dates_path_char));
		printf("Created \"%s\" file\n",dates_path_char);
		error_flag = true;
	} else {
		db_dates = read_dates(dates_path, &db_dates_index);
		db_dates_capacity = db_dates_index + 1 * 2;
	}

	if(members_path == NULL){
		printf("Couldent open \"%s\"\n",members_path_char);
		system(combine_with_space(MKFILE_COMMAND,members_path_char));
		printf("Created \"%s\" file\n",members_path_char);
		error_flag = true;
	} else { 
		db_members = read_members(members_path, &db_members_index);
		db_members_capacity = db_members_index + 1 * 2;
	}

	free(loans_path_char);
	free(books_path_char);
	free(dates_path_char);
	free(members_path_char);
	
	if(error_flag == true)
		return 1;
	
	printf("Imported %d objects from %s\n",db_loans_index + db_books_index + db_dates_index + db_members_index,DATA_DIR);
	return 0;
}

int dinit_db(){
	printf("Saving Database..\n");
	char *loans_path_char = combine(DATA_DIR,DATA_LOANS_FNAME);
	char *books_path_char = combine(DATA_DIR,DATA_BOOKS_FNAME);
	char *dates_path_char = combine(DATA_DIR, DATA_DATES_FNAME);
	char *members_path_char = combine(DATA_DIR, DATA_MEMBERS_FNAME);
	
	FILE *loans_path = fopen(loans_path_char, "rb");
	FILE *books_path = fopen(books_path_char, "rb");
	FILE *dates_path = fopen(dates_path_char, "rb");
	FILE *members_path = fopen(members_path_char, "rb");
	
	write_loans(loans_path,db_loans,db_loans_index);
	write_books(books_path,db_books,db_books_index);
	write_dates(dates_path,db_dates,db_dates_index);
	write_members(members_path,db_members,db_members_index);
	
	printf("Wrote %d objects to %s\n",db_loans_index + db_dates_index + db_dates_index + db_members_index, DATA_DIR);
	return 0;
}

// this is the setup function, it is what checks the 
// status of the database and handles flags.
int main(const int argument_count, const char *argument_list[]){ 
	
	// set a custom systemline argument
	argument* clean = NULL;
	set_argument(&clean,(set_argument_options){
			.FLAG_NAME = "--clean",
			.FLAG_CATAGORY = "base",
			.DESCRIPTION = "Destroy old data files (wipe the database)",
			.takes_value = 0,
			});

	argument *run = NULL;
	set_argument(&run,(set_argument_options){
			.FLAG_NAME = "--run",
			.FLAG_CATAGORY = "base",
			.DESCRIPTION = "Run the database code",
			.takes_value = 0,
			});

	// process arguments
	if(parse_arguments(argument_count,argument_list)==0){

		// catch signals
		signal(SIGINT,quit);

		
		
		// parse system arguments
		if(argument_run(clean)==0){
			if(!dir_exist(DATA_DIR)){
				printf("Database directoy dosent exist.\nThis means ther is nothing to clean\n");
				quit(0);
			}	

			char buff[SECURITY_CODE_LENGTH+1];

			// input prompt confirmation
			input(buff,SECURITY_CODE_LENGTH+1,"Do you really want to delete the database? (y/N): ");

			if(strcmp(buff,"y")==0 || strcmp(buff,"Y")==0){
				printf("To Confirm please write the following phrase into the input\n"
					"As soon as you press enter after putting in the folloing the database will be deleted\n");

				// generate confirmation code
				char rand[SECURITY_CODE_LENGTH + 1];
				for(int i=0;i<SECURITY_CODE_LENGTH;i++)
					printf("%c",rand[i] = random_char());
				printf("\n");
				rand[SECURITY_CODE_LENGTH] = '\0';

				// input for secuirty question
				input(buff,SECURITY_CODE_LENGTH+1,": ");

				if(strcmp((char *)buff,(char *)rand)==0){

					// delete the database folder
					system(combine_with_space(DELETE_COMMAND,DATA_DIR));
					printf("database deleted!\n");
				} else {
					printf("Wrong code entered\n");
					quit(1);
				}
			}
		}

		if(argument_run(run)==0){
			if(!dir_exist(DATA_DIR)){

				// create the database folder
				system(combine_with_space(MKDIR_COMMAND,DATA_DIR));
				printf("Created \"%s\" dir\n",DATA_DIR);
			}
		
			// import the database
			if(init_db() == 1){
				if(init_db() == 1)
					printf("Error occured with database\n");
			}

			// run the database function
			printf("DATABASE OUTPUT:\n\n");
			int ret = database();
			printf("\nEND OF DATABASE OUTPUT (exited with code %d)\n",ret);

			dinit_db();
		}
	} else phelp();
	quit(0);
}
