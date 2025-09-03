/* @file ui.c  
 * @breif mini ui library
 * @details this file contains all ui related functions used by the database 
 **/

#include "lib/hlib/lib/ansi.h"
#include "lib/lib.h"
#include <signal.h> 
#include <stdlib.h>
#include <string.h>
#include <stdio.h> 

#define DEFAULT_PROMPT "Use the up/down arrows to navigate, press enter to submit entry\n"

void ui_quit(int code){
	show_cursor();
	exit(code);
}

/** 
 * @breif wrapper function for ui_menu
 * @param array array of strings used in menu 
 * @param prompt prompt string shown at top of menu 
 * @return ui_menu 
 * @details wraps around ui_menu using a macro which removes the need for the 
 * 					user to enter the array size.
 **/
#define ui_m(array, prompt) ui_menu(array,sizeof(array)/sizeof(array[0]),prompt);

/** 
 * @brief draws a interactive menu 
 * @param *array[] array of strings used in menu 
 * @param size amount of items in *array[]
 * @param *prompt prompt strings shown at top of menu 
 * @return returns the selected index int of the menu
 * @details uses an array of strings as items and draws a menu, use of UPARROW 
 * 					and DOWNARROW can navigate the menu pressing enter returns the 
 * 					selected integer index of the array
 **/
int ui_menu(const char *array[], const int size, const char *prompt){
	hide_cursor();
	
	int selected = 0; 
	int longest = 0;

	// calulate the longest item in the list: 
	for(int i = 0; i < size; i++)
		if(strlen(array[i]) > longest) longest = strlen(array[i]);	

	while(1){
		clear();
		printf("%s",prompt);
		
		// draw the menu 
		for(int i = 0; i < size; i++){
			printf(" %2d. %s",i+1,array[i]);
			
			// draw some white space 
			for(int j = 0; j < longest - strlen(array[i]); ++j) printf(" ");
			if(i == selected) printf(" <\n");
			else printf("  \n");
		}


		// handle the input
		int ch = achar();
		switch(ch){
			case 'B':
				if(selected==size-1) selected = 0;
				else selected++;
				break;
			
			case 'A':
				if(selected==0) selected=size-1;
				else selected--;
				break;

			case 10:
				show_cursor();
				return selected;
		}
	}

	show_cursor();
	return -1;
}



int ui_main(){
	// main menu
	const char *main_menu[] = {
		"Create Data",
		"Read Data",
		"Update Data", 
		"Delete Data",
	};

	const char *create[] = {
		"Member",
		"Book",
		"Loan",
	};

	const char *read[] = {
		"Members",

	};


	int ret = ui_m(main_menu,"Please select the database macro\n");
	return 0;
}


int ui_test(){
	signal(SIGINT,ui_quit);

	// define items for the menu
	const char *list[] = {
		"one",
		"two",
		"three",
		"four",
		"five",
		"six",
		"seven",
		"eight",
		"nine",
		"ten",
	};
	
	// use the menu function to ask the user for input
	const int sel = ui_m(list,DEFAULT_PROMPT);

	// display the chosen item
	printf("You picked %s!\n",list[sel]);
	
	return 0;
}

