/*
TODO: 
for some reason the program crashes on FIRST write, i think this has something to do with 
the members write function but im not sure. 

finish writing the wizard functions.
*/

// 3rd party imports
#include "lib/data.h"
#include "lib/lib.h"
#include "lib/plib.h"
#include "lib/hlib/lib/ansi.h"
#include "db.c" // actual database file
//#include "ui.c" // interface file

// standard librarys 
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>


void quit(int code){
	printf("\rexiting program with code %d ..\n",code);
	show_cursor();
	dinit_argument_list();
	exit(code);
}

// status of the database and handles flags.
int main(const int argument_count, const char *argument_list[]){ 
	if(PLATFORM_NOT_SUPPORTED){
		printf("your platform is not supported\neither use _WIN32 or __unix__ platforms\n");
		exit(-1);
	}

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
			.FLAG_CATAGORY = "base",
			.DESCRIPTION = "View database information",
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

		if(!dir_exist(DATA_DIR)){
			printf("Couldent open \"%s\"\n",DATA_DIR);
			// create the database folder
			char *com = combine_with_space(MKDIR_COMMAND,DATA_DIR);
			system(com);
			free(com);
			printf("Created \"%s\" dir\n",DATA_DIR);
		}
		
		// import the database
		switch(init_db()){
			case 1:

				// re-import the database 
				if(init_db() == 1)
					printf("Error occured with database, unsure of cause\n");
					exit(1);
				break;

			case -5: 
				printf("fatal database import error\n");
				exit(-5);
				break;

			default: 
				break;
		}

		if(argument_run(run)==0){
			printf("DATABASE OUTPUT:\n\n"); 
			int ret = database();
			printf("\nEND OF DATABASE OUTPUT (exited with code %d)\n",ret);
			fflush(stdout);
		}

		// save the database 
		//dinit_db();
		free(db_members);
    free(db_loans);
    free(db_books);
    db_members = NULL;
    db_loans = NULL;
    db_books = NULL;
	} else phelp();
	quit(0);
}
