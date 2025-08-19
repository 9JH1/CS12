#include "data.h"
#include "lib/lib.h" 
#include "lib/plib.h"

#include <stdlib.h>
#include <stdio.h> 
#include <signal.h> 
#include <string.h> 


#define SECURITY_CODE_LENGTH 10
#define DATA_DIR "./db/"

void quit(int code){
	printf("\rexiting program with code %d..\n",code);
	exit(code);
}


int main(const int argument_count, const char *argument_list[]){ 
	argument* clean = NULL;
	set_argument(&clean,(set_argument_options){
			.FLAG_NAME = "--clean",
			.FLAG_CATAGORY = "base",
			.DESCRIPTION = "Destroy old data files (wipe the database)",
			.takes_value = 0,
			});
	
	if(parse_arguments(argument_count,argument_list)==0){
		signal(SIGINT,quit); // enable signal handler

		if(argument_run(clean)==0){
			char buff[100];
			input(buff,100,"Do you really want to clean db? (y/N): ");
			if(strcmp(buff,"y")==0 || strcmp(buff,"Y")==0){
				printf("To Confirm please write the following phrase into the input\n");

				// show a random list of letters 
				char rand[SECURITY_CODE_LENGTH + 1];
				for(int i=0;i<SECURITY_CODE_LENGTH;i++)
					printf("%c",rand[i] = random_char());
				printf("\n");
				rand[SECURITY_CODE_LENGTH] = '\0';

				// take input
				input(buff,100,": ");
				if(strcmp((char *)buff,(char *)rand)==0){
					printf("cleaning database\n");
				} else {
					printf("Wrong code entered\n");
					quit(1);
				}
			}
		}
	} else phelp();

	dinit_argument_list();
	quit(0);
}
