/* 
 * @file main.c 
 * @breif handles main scripting. 
 */

#include "include.h"
#include "src/remote/plib/plib.h"

/* 
 * @breif custom exit function 
 * @description useful for on-exit operations
 * @param code exit code 
 */
void quit(int code){
	printf("\rexiting db with code %d..\n",code);
	exit(code);
}


/*
 * @breif custom help function 
 */ 
void help(){
	printf("db [options]\n");
	pl_help();
}

typedef enum {
	SUCCESS = 0,
	FAIL = 1,
	HELP_MENU_REF = 2,
} return_codes;

/* 
 * @breif main database loop
 * @description this function is the first bit of 
 *              code run in the database, it handles 
 *              all of the argument declarations,
 *              initialization and many other functions 
 * @param c the count of system arguments 
 * @param v an array of system arguments
 */
int main(const int c, const char *v[]){
	pl_arg *db_help = PL_P("--help","show this dialog");
	pl_arg *db_init = PL_P("--init", "initialize the database");
	pl_arg *db_delete = PL_P("--clean","remove the database");
	pl_arg *db_no_write = PL_P("--nowrite","remove write privleges from current user");
	pl_r ret;

	if((ret = pl_proc(c,v)) == PL_SUCCESS){
		printf("Hello World!");
	} else { // handle errors
		help();
		printf("error: '%s' from argument '%s'\n",
				PL_E(ret), // error code stringify'd
				PL_LAST_ARG); // the last arg parsed in plib 
		quit(HELP_MENU_REF);
	}

	quit(SUCCESS);
}

