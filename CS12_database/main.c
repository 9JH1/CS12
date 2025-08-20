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

// actual local database 
loan *db_loan;
int db_loan_index; 
int db_loan_capacity;

Member *db_members;
int db_members_index;
int db_members_capacity;

book *db_books;
int db_books_index;
int db_books_index;

// load the database
int init_db(){
	
	return 1;
}


void quit(int code){
	if(code != 0)
		printf("\rexiting program with code %d..\n",code);
	dinit_argument_list();
	exit(code);
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
		bool data_dir_created = false;
		if(!dir_exist(DATA_DIR)){

			// create the database folder
			int size = snprintf(NULL,0,"%s %s",MKDIR_COMMAND,DATA_DIR);
			char com[size+1];
			sprintf(com, "%s %s",MKDIR_COMMAND, DATA_DIR);
			printf("Creating missing \"%s\" dir\n",DATA_DIR);
			system(com);
			data_dir_created = true;
		}
		
		// parse system arguments
		if(argument_run(clean)==0){
			if(data_dir_created){
				printf("the data dir was just created therefore there is no data to remove\n");
				quit(0);
			}
			
			// take input
			char buff[100];
			input(buff,100,"Do you really want to delete the database? (y/N): ");
			if(strcmp(buff,"y")==0 || strcmp(buff,"Y")==0){
				printf("To Confirm please write the following phrase into the input\n"
					"As soon as you press enter after putting in the folloing the database will be deleted\n");

				// generate confirmation code
				char rand[SECURITY_CODE_LENGTH + 1];
				for(int i=0;i<SECURITY_CODE_LENGTH;i++)
					printf("%c",rand[i] = random_char());

				printf("\n");
				rand[SECURITY_CODE_LENGTH] = '\0';
				input(buff,100,": ");
				if(strcmp((char *)buff,(char *)rand)==0){

					// delete the database folder
					int size = snprintf(NULL,0,"%s %s",DELETE_COMMAND,DATA_DIR);
					char com[size+1];
					sprintf(com,"%s %s",DELETE_COMMAND,DATA_DIR);
					printf("deleting database...\n");
					system(com);
					printf("database deleted!\n");
					quit(0);
				} else {
					printf("Wrong code entered\n");
					quit(1);
				}
			}
		}
		if(argument_run(run)==0){
			printf("database exited with code %d\n",database());
		}

		// \/ call help function
	} else phelp();

	// exit with code 0
	quit(0);
}
