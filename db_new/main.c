/** 
 * @file main.c 
 * @brief Handles main scripting. 
 **/
#include "include.h"
#include "src/remote/plib/plib.h"

/**
 * @brief main database loop
 * @param c the count of system arguments 
 * @param v an array of system arguments
 *
 * this function is the first bit of 
 * code run in the database, it handles 
 * all of the argument declarations,
 * initialization and many other functions 
 **/
int main(const int c, const char *v[]){
	pl_arg *db_help = PL_P("--help","show this dialog");
	pl_arg *db_init = PL_P("--init", "initialize the database");
	pl_arg *db_delete = PL_P("--clean","remove the database");
	pl_arg *db_no_write = PL_P("--nowrite","remove write privleges from current user");
	pl_r ret;

	if((ret = pl_proc(c,v)) == PL_SUCCESS){
		printf("Hello World!\n");
	} else { // handle errors
		help();
		printf("error: '%s' from argument '%s'\n",
				PL_E(ret), // error code stringify'd
				PL_LAST_ARG); // the last arg parsed in plib 
		quit(HELP_MENU_REF);
	}

	quit(SUCCESS);
}

