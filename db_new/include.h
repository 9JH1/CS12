/** 
 * @file include.h 
 * @description this file is the global import header which imports all librarys to 
 *        the main files. 
 **/
#ifndef INCLUDE 
#define INCLUDE 

// third party imports
#include "src/remote/plib/plib.h"
#include "src/platform.h"

// standard imports
#include <stdio.h>
#include <stdlib.h>


/**
 * @breif custom exit function 
 * @description useful for on-exit operations
 * @param code exit code 
 **/
void quit(int code);


/**
 * @breif custom help function 
 **/ 
void help(void);


/**
 * @breif return codes for main program 
 **/
typedef enum {
	SUCCESS = 0,
	FAIL = 1,
	HELP_MENU_REF = 2,
} return_codes;


#endif
