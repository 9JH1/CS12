/*
 * @file lib.c 
 * @breif this file contains all of the function definitions from in 
 *        @ref include.h.
 */
#include "../include.h"



void quit(int code){
	printf("\rexiting db with code %d..\n",code);
	exit(code);
}


void help(){
	printf("db [options]\n");
	pl_help();
}
