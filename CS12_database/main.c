#ifdef PLATFORM_NOT_SUPPORTED
#include <stdio.h>
printf("this platform is not supported, use either Linux (__unix__) or windows (_WIN32)\n");
#else

// 3rd party imports
#include "lib/data.h"
#include "lib/lib.h" 
#include "lib/plib.h"
#include "db.c"

// standard librarys 
#include <stdlib.h>
#include <stdio.h> 
#include <signal.h> 
#include <string.h> 
#include <stdbool.h>


// actual local database 
loan *db_loans;
int db_loans_index; 
int db_loans_capacity;

member *db_members;
int db_members_index;
int db_members_capacity;

book *db_books;
int db_books_index;
int db_books_capacity;

void quit(int code){
	printf("\rexiting program with code %d ..\n",code);
	dinit_argument_list();
	exit(code);
}

// status of the database and handles flags.
int main(const int argument_count, const char *argument_list[]){ 
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

	argument *view = NULL;
	set_argument(&view, (set_argument_options){
			.FLAG_NAME = "--view",
			.FLAG_CATAGORY = "db_view",
			.DESCRIPTION = "View database information",
			.takes_value = 0,
			});

	argument *db_member_wizard = NULL;
	set_argument(&db_member_wizard,(set_argument_options){
			.FLAG_NAME = "--member-wizard",
			.FLAG_CATAGORY = "db_create",
			.DESCRIPTION = "Run the member wizard",
			.takes_value = 0,
			}); 

	argument *db_loan_wizard = NULL;
	set_argument(&db_loan_wizard,(set_argument_options){
			.FLAG_NAME = "--loan-wizard",
			.FLAG_CATAGORY = "db_create",
			.DESCRIPTION = "Run the loan wizard",
			.takes_value = 0,
			});

	argument *db_book_wizard = NULL;
	set_argument(&db_book_wizard,(set_argument_options){
			.FLAG_NAME = "--book-wizard",
			.FLAG_CATAGORY = "db_create",
			.DESCRIPTION = "Run the book wizard",
			.takes_value = 0,
			});

	argument *db_member_update_wizard = NULL;
	set_argument(&db_member_update_wizard,(set_argument_options){
			.FLAG_NAME = "--member-update-wizard",
			.FLAG_CATAGORY = "db_update",
			.DESCRIPTION = "Update an existing member",
			.takes_value = 0,
			});

	argument *db_loan_update_wizard = NULL;
	set_argument(&db_loan_update_wizard,(set_argument_options){
			.FLAG_NAME = "--loan-update-wizard",
			.FLAG_CATAGORY = "db_update",
			.DESCRIPTION = "Update an existing loan",
			.takes_value = 0,
			});

	argument *db_book_update_wizard = NULL;
	set_argument(&db_book_update_wizard,(set_argument_options){
			.FLAG_NAME = "--book-update-wizard",
			.FLAG_CATAGORY = "db_update",
			.DESCRIPTION = "Update an existing book",
			.takes_value = 0,
			});

	argument *db_member_delete = NULL;
	set_argument(&db_member_delete,(set_argument_options){
			.FLAG_NAME = "--member-delete",
			.FLAG_CATAGORY = "db_delete",
			.DESCRIPTION = "Delete a member from the database",
			.takes_value = 0,
			});

	argument *db_loan_delete = NULL;
	set_argument(&db_loan_delete,(set_argument_options){
			.FLAG_NAME = "--loan-delete",
			.FLAG_CATAGORY = "db_delete",
			.DESCRIPTION = "Delete a loan from the database",
			.takes_value = 0,
			});

	argument *db_book_delete = NULL;
	set_argument(&db_book_delete,(set_argument_options){
			.FLAG_NAME = "--book-delete",
			.FLAG_CATAGORY = "db_delete",
			.DESCRIPTION = "Delete a book from the database",
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
					char *com = combine_with_space(DELETE_COMMAND,DATA_DIR);
					system(com);
					free(com);
					printf("database deleted!\n");
				} else {
					printf("Wrong code entered\n");
					quit(1);
				}
			}
		}

		if(argument_run(run)==0){
			if(!dir_exist(DATA_DIR)){
				printf("Couldent open \"%s\"\n",DATA_DIR);
				// create the database folder
				char *com = combine_with_space(MKDIR_COMMAND,DATA_DIR);
				system(com);
				free(com);

				printf("Created \"%s\" dir\n",DATA_DIR);
			}
		
			// import the database
			if(init_db() == 1){
				// re-import the database 
				if(init_db() == 1)
					printf("Error occured with database, unsure of cause\n");
			}

			// run the database function
			printf("DATABASE OUTPUT:\n\n"); 
			int ret = database();
			printf("\nEND OF DATABASE OUTPUT (exited with code %d)\n",ret);
			fflush(stdout);

			// write the database 
			dinit_db();

			// free the members 
			for(int i =0;i < db_members_index;i++){
				member *m = &db_members[i];

				// the following causes a double free error for some reason, i couldent find the source
				/*if(m->first_name != NULL) free(m->first_name);
  			if(m->last_name != NULL) free(m->last_name);
  			if(m->email != NULL) free(m->email);
  			if(m->phone_number != NULL) free(m->phone_number);
  			if (m->loan.loan_ids) free(m->loan.loan_ids);
  			if (m->type == AUTHOR && m->o.author.genre) free(m->o.author.genre);*/
			}
		}
	} else phelp();
	quit(0);
}
#endif
