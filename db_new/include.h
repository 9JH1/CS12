/** 
 * @file include.h 
 * @brief contains all function definitions
 * This file includes every function, external variable, structure and enumerator 
 * used through out all of the code. this function is imported to most child files 
 * which means that the includes can be moderated very easily.
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
 * @brief custom exit function 
 * @description useful for on-exit operations
 * @param code exit code 
 *
 **/
void quit(int code);


/**
 * @brief custom help function 
 * custom help function, this is where all of the 
 **/ 
void help(void);


/**
 * @brief return codes for main program 
 **/
typedef enum {
	SUCCESS = 0,
	FAIL = 1,
	HELP_MENU_REF = 2,
} return_codes;


#endif
