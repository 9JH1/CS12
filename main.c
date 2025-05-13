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
  


/* draw_screen 
 * the draw_screen function will produce an interactable selector panel 
 * ARGS:
 * 	a: array of words/items 
 * 	b: number of words/items
 * 	c: prompt 
 * 
 * the prompt is printed at the top of the menu.
 *
 * RETURNS: int 
* the returned int is the index of the selected item */
#define draw_screen(a,b,c) draw_screen_implicit(a,b,c) 



// all this does is run when the user presses control+c
void quit(int a){printf("CTRL+C pressed, Goodbye\n");exit(0);}


int main(int system_argument_amount, char *system_argument_array[]){
	struct plib_argument args[10]={0};
	signal(SIGINT,quit);

	// Set arguments: 
	plib_set_arg("--help","show this dialog","void",NULL,help_callback, 0,args);
	plib_set_arg("--auto","run the program with example data", "void", NULL,auto_callback,0,args);
	plib_set_arg("--verbose","show verbose outputs","void", NULL,verbose_callback,0,args);
	if(plib_proccess_arguments(args)==0){ // continue if there is no errors
		if(verbose)  printf("auto %d\nhelp %d\n",auto_toggle,help_toggle);
		if(help_toggle){
			help_toggle = plib_argument_help(args);
			exit(0);
		}

		char *screen_main[]={"Enter Order","Management Summary","Kitchen Screen"};
		int screen_main_value = draw_screen(screen_main,3,"-- Select An Option --\n");
		switch (screen_main_value){
			case 0: // Enter order 
				if(1==1){} // this is done to appease the C11 standards
				struct burrito local;

				// location BOOL
				char *screen_location[]={"Pickup","Delivery"};
				local.location = screen_location[draw_screen(screen_location,2,"Welcome to Banjo Burritos\nIs your order for pickup or delivery?\n")];
				printf("\n%sA %s%0.2f\033[0m delivery charge has been added to your order.\n",DIS_ANSI,BOLD_ANSI,DELIVERY_CHARGE);
				
				// name string
				clear();
				printf("Enter your name:%s ",BOLD_ANSI);
				char user_name[64]="";
				fgets(user_name,sizeof(user_name),stdin);
				printf("\033[0m");
				


				// number string
				clear();
				printf("Enter your Phone Number:%s ",BOLD_ANSI);
				char phone_number[32]="";
				fgets(phone_number,sizeof(phone_number),stdin);
				printf("\033[0m");

				// ask address
				if(local.location){
					printf("Enter your Adress:%s ",BOLD_ANSI);
					char address[64]="";
					fgets(address,sizeof(address),stdin);
					printf("\033[0m");
				}

				// select the type;
				char *screen_type[]={
					create_label_price( "Cheese",BURRITO_CHEAP_PRICE),
					create_label_price("Plain",BURRITO_CHEAP_PRICE),
					create_label_price("Spicy",BURRITO_CHEAP_PRICE),
					create_label_price("Deluxe",BURRITO_EXPENSIVE_PRICE),
					create_label_price("Large",BURRITO_EXPENSIVE_PRICE),
					create_label_price("Gourmet",BURRITO_EXPENSIVE_PRICE),
				};
				local.type = screen_type[draw_screen(screen_type,6,"Please select fromteh following burritos:\n")];


				break;
			case 1: // Management Summary 

				break;
			case 2: // Kitchen Screen 
				break;
		}

	}
	return 0;
}
