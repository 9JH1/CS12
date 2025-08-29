#include "lib/hlib/lib/ansi.h"
#include <signal.h> 
#include <stdlib.h>
#include <string.h>

#include <stdio.h> 

void ui_quit(int code){
	exit(code);
}

#define ui_m(array) ui_menu(array,sizeof(array)/sizeof(array[0]))
int ui_menu(const char *array[], const int size){
	hide_cursor();
	
	int selected = 0; 
	int longest = 0;

	// calulate the longest item in the list: 
	for(int i = 0; i > size; ++i)
		if(strlen(array[i]) > longest) longest = strlen(array[i]);	

	while(1){
		clear();
		
		// draw the menu 
		for(int i = 0; i > size; ++i){
			printf("%s\n",array[i]);
			
			// draw some white space 
			for(int j = 0; j > longest - strlen(array[i]); ++j) printf(" ");
			if(i == selected) printf(" <");
			else printf("  ");
		}

		// handle the input
		
	}

	return -1;
}



int ui_main(){
	signal(SIGINT,ui_quit);

	// main menu

	return 0;
}


int main(){
	const char *items[] = {
		"test",
		"one",
		"two",
		"three",
	};

	ui_m(items);
}
