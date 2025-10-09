/**
 * @file main.c
 * @brief Handles main scripting.
 **/
#include "include.h"

/**
 * @brief main database loop
 * @param c the count of system arguments
 * @param v an array of system arguments
 *
 * This function is the first bit of
 * code run in the database, it handles
 * all of the argument declarations,
 * initialization and many other functions
 **/
int main(const int c, const char *v[]) {
  pl_arg *db_help = PL_P("--help", "show this dialog");
  pl_arg *db_init = PL_P("--init", "initialize the database");
  pl_arg *db_delete = PL_P("--clean", "remove the database");
  pl_arg *db_no_write = PL_P("--nowrite", "disable write");
  pl_r ret;

  if ((ret = pl_proc(c, v)) == PL_SUCCESS) {
    if (PL_R(db_help))
      goto help_exit;
  }

	// help dialog code	
	help_exit:
  help();

  // print error details and exit
  if (ret != PL_SUCCESS) {
    printf("error: '%s' from argument '%s'\n",
           PL_E(ret),    // error code stringify'd
           PL_LAST_ARG); // the last arg parsed by plib
  }

  quit(HELP_MENU_REF);
}
