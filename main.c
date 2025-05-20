/* Simple Burrito ordering system 
 * Lukan Snopovs (c) 2025 
 * --------------------------
 * view LICENCE for more details */


#include <stdio.h> // printf 
#include <string.h> // strlen strcmp
#include <stdlib.h> // malloc realloc 
#include <unistd.h> // exit 
#include "main_lib.c" // everything else


// imported functions
#define draw_screen(a, b, c) draw_screen_implicit(a, b, c) 
#define set_argument(a, b, c, d, e, f, g) plib_set_arg(a, b, c, d, e, f, g)
#define proccess_arguments(a) plib_proccess_arguments_implicit(system_argument_amount,system_argument_array,a)
#define draw_header() draw_header_implicit()
#define handle_quit() handle_quit_implicit()
#define draw_header_sep() draw_header_sep_implicit()


/* draw_management_screen:
 * ARGS:
 * 	order_list: a list of Burrito orders. 
 * 	order_index: the amount of items in order_list
 * 
 * RETURNS:
 * 	nothing.
 *
 * DESCRIPTION:
 *	this function uses the burrito list to generate and display 
 *	different data. some of the data used is the sum price and 
 *	amount of orders made. */
void draw_mangement(Burrito *order_list, int order_index){
	float total_sales=0;
	int pickup=0, 
			delivery=0, 
			dine_in=0;
	char pickup_suffix[1]="",
			 delivery_suffix[1]="",
			 dine_in_suffix[1]="",
			 total_suffix[1]="";

	// Loop through and calculate sums
	for(int i=0;i<order_index;i++){
		if(order_list[i].mode == PICKUP) pickup++;
		else if (order_list[i].mode == DELIVERY) delivery++;
		else if (order_list[i].mode == DINEIN) dine_in++;				
		total_sales+=order_list[i].price;
	}
	
	// Add suffixes
	if(pickup > 1) pickup_suffix[0] = 's';
	if(delivery > 1) delivery_suffix[0] = 's';
	if(dine_in > 1) dine_in_suffix[0] = 's';
	if(dine_in>1 || pickup>1 || delivery>1) total_suffix[0] = 's';

	// draw the ui
	draw_header_sep();
	printf("there have been a total of %s%d\033[0m customer%s with a order sum price of %s$%0.2f\033[0m\n",BOLD_ANSI,pickup+delivery+dine_in,total_suffix,BOLD_ANSI,total_sales);
	if(pickup > 0){
		printf("%s%d\033[0m customer%s picked up their order",BOLD_ANSI,pickup,pickup_suffix);
	}
	
	if(delivery > 0){
		printf(", %s%d\033[0m customer%s picked up their order",BOLD_ANSI,delivery,delivery_suffix);
	}

	if(dine_in){
		printf(", %s%d\033[0m customer%s picked up their order",BOLD_ANSI,dine_in,dine_in_suffix);
	}
	printf(".\n");

	draw_header_sep();
	printf("%sPress %sany key\033[0m%s to go back to main menu.\033[0m\n",DIS_ANSI,BOLD_ANSI,DIS_ANSI);
	achar();
}


/* input: 
 * ARGS:
 * 	prompt: printed at the top of the selector menu 
 * 	keyword: keyword of the prompt eg USERNAME or PASSWORD 
 * RETURNS:
 * 	char of undefined size, program returns the string of the 
 * 	input that the user puts. 
 * DESCRIPTION:
 * 	this function very simply asks the user to enter input,
 * 	this input function has built in themeing and blank input 
 * 	checking */
char* input(char *prompt,char *keyword){
	while(1){
		clear();
		draw_header();

		// print the prompt
		printf("%s %s:%s%s ",prompt,keyword,BOLD_ANSI,SEL_ANSI);
		static char input[256]="";
		
		// take the input
		fgets(input,sizeof(input),stdin);
		printf("\033[0m");
		input[strcspn(input, "\n")] = 0;
		// error check and return 
		if(strcmp(input,"\n")!=0 && strlen(input)>0){
			return input;
		} else {
			printf("%sPlease Enter your %sACTUAL %s\033[0m%s.\033[0m\n",DIS_ANSI,BOLD_ANSI,keyword,DIS_ANSI);
			sleep(USER_SLEEP_DELAY);
		}
	}
}



/* draw_screen:
 * ARGS:
 * 	array: a array of words used as items in the menu 
 * 	array_length: the amount of items in array.
 * 	prompt: text printed at the top of the menu 
 * RETURNS:
 * 	int, program returns the index of the current selected 
 * 	item. 
 * DESCRIPTION:
 * 	this function takes an array of words and uses those words 
 * 	to generate an interactive menu that lets the user use the 
 * 	arrow keys to choose options from the array of words, when 
 * 	the user presses enter the function returns the index of 
 * 	the selected value. */
int draw_screen(char *array[],size_t array_length,char *prompt){
	int selected=0;
	clear();
	while (1){
		clear_a();
		draw_header();
		printf("%s\n",prompt);

		// enumerate through the words 
		for(int i=0;i<array_length;i++){
			if(selected == i){
				printf("%s%s>",SEL_ANSI,BOLD_ANSI);
			} else printf(" ");
			printf(" %s\033[0m\n",array[i]);
		}

		// handle the user input
		int ch = achar();
		if(ch == 10) break;
		
		// simple switch case
		switch(ch){
			case 'B':
				if(selected==array_length-1) selected=0;
				else selected++;
				break;
			case 'A':
				if(selected==0) selected=array_length-1;
				else selected--;
				break;
		}
	}
	draw_header_sep();
	printf("\nYou selected %s%s\033[0m\n",BOLD_ANSI,array[selected]);
	fflush(stdout);
	sleep(USER_SLEEP_DELAY);
	return selected;
}

// system_argument_count and system_argument_array are the system vars parsed into the function on run
int main(int system_argument_amount, char *system_argument_array[]){

	// Set arguments & Quit Handler 
	struct plib_argument args[3] = {0};
	set_argument("--help","show this dialog","void",NULL,help_callback, 0,args);
	set_argument("--verbose","show extra information","void",NULL,verbose_callback,0,args);
	
	/* this small line basically just binds a killcode to a function, in this case it will 
	 * run the quit() function if the user presses Control+C while the program is running,
	 * this is useful as we can free memory and perform other clean up before we close the 
	 * script, and yes this method can be abused into making a non quitable file ( on windows 
	 * at least ) */ 
	handle_quit();

	/* the next line runs the implicit plib_process_arguments function, what this does is 
	 * it preloads and runs callbacks and other functions based off of the arguments run 
	 * with the program eg --help and --verbose. */
	if(proccess_arguments(args) == 0){
		Burrito *order_list = malloc(orders_capacity * sizeof(Burrito));
			
		/* checking if order list is defined is important as if it is not it shows that a 
		 * potential memory leak has occured. this can occur when order_capacity or the 
		 * Burrito struct are incorrectly set */
		if(!order_list){
			printf("Error!! memory leak occured!!! the burritos are free!!!!\n");
			return 1;
		}
		
		/* unlike python C does not come with booleans unless you import another library,
		 * instead of true and false C uses 1 and 0 this is why you will see int value returns 
		 * and conditinals throughout the code. */

		while(1){
			char *screen_main[]={"Enter Order","Management Summary","Kitchen Screen","Exit"};
			int screen_main_value = draw_screen(screen_main,4,"-- Select An Option --");

			/* after viewing the documentation on draw_screen you can see it returns a int 
			 * value corrasponding to the selection menu index, i've decided to use a switch 
			 * case block as i can easily set different outputs based off of the menu item the 
			 * user selects. */
			switch (screen_main_value){
				case 0:
					// make a new callback point (used later on if the user wants to restart order)
					re_order:
					if(order_index == orders_capacity){

						/* this if block will dynamically allocate and re-allocate memory to fit more orders 
						 * if the current order was about to overflow in the order_list malloc this block simply 
						 * adds more space to order_list. */
						if(verbose){
							printf("orders struct has been reallocated from %d (%d bytes) -> %d (%d bytes).\n",orders_capacity,orders_capacity*(int)sizeof(Burrito),orders_capacity*2,(orders_capacity*2)*(int)sizeof(Burrito));

							sleep(USER_SLEEP_DELAY);
						}
						
						// times the capacity by two so 1, 2, 4, 8, 16... 
						orders_capacity *= 2;
						Burrito *temp = realloc(order_list,orders_capacity * sizeof(Burrito));

						/* again error checking to make sure that no issues occured when allocating memory 
						 * a good example of why this is necercerry is if the system is out of memory and the program 
						 * types to allocate memory that the system doesnt have, that could cause fatal errors and ==
						 * other undefined behaviour. */
						if(!temp){
							if(verbose) printf("Error occured in order_list Realloc function\n");
							free(order_list);
							return 1;
						} else order_list = temp;
					}

					/* create a new local order, this is nice and easy as instead of having to call values like 
					 * order_list[order_index].<value> it can simply be called using order.<value>.
					 *
					 * Also, because order is now a pointer and an undefined (*) pointer, it means that instead 
					 * of using the conventional fullstop to query a value we use the -> operator, so  for 
					 * future reference order->value is the same as order.value */
					Burrito *order = &order_list[order_index];

					// initialize price value (asssigned value later on)
					order->price = 0;
					char *screen_location[]={"Pickup","Delivery","Dine In",};

					/* I decided to use something ive never used before for this particular data type, that being 
					 * an ENUM data type, similar to a struct you can set const values easily, instead of having to 
					 * allocate memory for three porentail different words or having a hard to read int value corralating 
					 * to string values i can instead define three constants that can be used like ints eg one of these
					 * three consts is DELIVERY and i can also call it using the int 1*/
					order->mode = (OrderMode)draw_screen(screen_location,3,"Is your order for pickup or delivery?");

					// See? easy to read
					if(order->mode==DELIVERY){
						printf("%sA %s%0.2f\033[0m%s delivery charge has been added to your order.\033[0m\n",DIS_ANSI,BOLD_ANSI,DELIVERY_CHARGE,DIS_ANSI);
						
						// Add a charge
						order->price += DELIVERY_CHARGE;
						order_price_global+=DELIVERY_CHARGE;
						sleep(USER_SLEEP_DELAY);	
					}

					// get other basic input info 
					order->name = input("Please enter your","name");
					order->number = input("Please enter your","number");

					// only ask for address if the location mode is delivery 
					if(order->mode == DELIVERY) order->address = input("Please enter your","address");

					/* now this might look like a rather insignificant line but really this line calls the main 
					 * attraction, that being the burrio selector program, this function is absolutly disgusting 
					 * to look at even though it is as pretty as i could make it which is why it is not included 
					 * in this release of the code. */
					order->type = display_burrito_menu();

					/* the display_burrito_menu returns a submenu of allocated memory, this submenu contains a 
					 * list of every burrito type, price and amount the user ordered, this simple loop adds up 
					 * the sum of all these values this number is used in various display elements. */
					for(int i=0;i<BURRITO_TYPE_AMOUNT;i++){
						order->price += order->type[i].amount*order->type[i].price;
					}

					// Draw the final screen 
					char *screen_complete_order[]={"Yes","No, Restart Order",};
					char price_header[256];
					sprintf(price_header,"Final Price: %s$%0.2f\033[0m, Confirm Order?",BOLD_ANSI,order_price_global);
					
					/* this conditional looks complex but simply will run if the user DOES NOT enter a value of 1
					 * as one would evaluate to true which would pass the conditional. */ 
					if(!draw_screen(screen_complete_order,2,price_header)){
						
						// clean up for next frame
						order_index++;
						order_price_global=0;
						order_amount_global=order_index;
					} else {

						// go back to the start to redefine the values
						printf("Restarting order.\n");
						sleep(USER_SLEEP_DELAY);
						goto re_order;
					}
					break;

				case 1:
					if(order_index>0) draw_mangement(order_list,order_index);
					else {
						printf("No orders to view\n");
						sleep(USER_SLEEP_DELAY);
					}
					break;

				case 2:
					draw_kitchen_screen(order_list,order_index);
					break;

				case 3:
					if(0){}

					int memory_sum = 0;
					// loop through and free all nested malloc'd memory
					for(int i=0;i<order_index;i++){
						if(verbose){
							memory_sum += ((orders_capacity*sizeof(Burrito_type))/order_index);
							printf("Free'd total %d bytes from nested orders\n",memory_sum);
						}
						free(order_list[i].type);
					}

					// free the list and exit the program :D
					if(verbose){
						memory_sum += orders_capacity*sizeof(Burrito);
						printf("Free'd total %d bytes of memory from order_list\n",memory_sum);
					}
					free(order_list);
					return 0;
			}
		}
	}
	return 0;
}
