/** 
 * @file include.h 
 * @brief contains all function definitions
 *
 * This file includes every function, external variable, structure and enumerator 
 * used through out all of the code. this function is imported to most child files 
 * which means that the includes can be moderated very easily.
 **/
#ifndef INCLUDE 
#define INCLUDE 

// Third party
#include "src/remote/plib/plib.h"
#include "src/platform.h"

// Standard
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Custom exit function
 * @param code Exit code
 **/
void quit(int code);

/**
 * @brief Custom help function, mostly just a @ref pl_help wrapper
 **/ 
void help(void);

/**
 * @brief Return codes for main program
 **/
typedef enum {
	SUCCESS = 0,
	FAIL = 1,
	HELP_MENU_REF = 2,
} return_codes;
#endif
