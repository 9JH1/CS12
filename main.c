/*
  MIT License

  Copyright (c)  Lukan Snopovs 2024-2025

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

	-----------------------------------------------------------------------------
	This file is associated with the 2025 Yr 12 Computer science assessment.

*/
 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "main_lib.c" // library

// set up signal and arguments
int main(int system_argument_amount, char *system_argument_array[]){

	// -------------- define arguments ---------------
	struct plib_argument args[10]={0};
	plib_set_arg(
			"--auto",
			"run the program with example data",
			"void",
			NULL,
			auto_callback,
			0,
			args
			);

	plib_set_arg(
			"--help",
			"show this dialog",
			"void",
			NULL,
			help_callback,
			0,
			args
			);

	plib_set_arg(
			"--verbose",
			"show verbose outputs",
			"void",
			NULL,
			verbose_callback,
			0,
			args
			);
	
	// -------------------- run ----------------------
	if(plib_proccess_arguments(args)==0){
		// continue without errors
		
		if(verbose)  printf("auto %d\nhelp %d\n",auto_toggle,help_toggle);
		
		// refer to main_lib for help_toggle definition
		if(help_toggle){
			help_toggle = plib_argument_help(args);
			exit(0);
		}

		// entered clean main snippet 	
		char *screen_main[]={
			"Enter Order",
			"Management Summary",
			"Kitchen Screen",
		};
		
		// draw the first menu
		int screen_main_value = draw_screen(
				screen_main,
				3,
				"-- Select An Option --\n"
				);

		switch (screen_main_value){
			case 0: // Enter order 
				printf("You entered %s%s\033[0m\n",BOLD_ANSI,screen_main[screen_main_value]);
				sleep(USER_SLEEP_DELAY);
				char *screen_location[]={
					"Pickup",
					"Delivery",
				};

				// draw the second menu
				int screen_location_value = draw_screen(
						screen_location,
						2,
						"Welcome to Banjo Burritos/nIs your order for pickup or delivery?"
						);

				switch (screen_location_value){
					case 0:
						break;
					case 1: 
						break;
				}
				
				break;
			case 1: // Management Summary
				break;
			case 2: // Kitchen Screen 
				break;
		}

	}
	return 0;
}
