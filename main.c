#include <stdio.h>
#include "main_lib.c" // dependancy

// constants
#define draw_screen(a, b, c) draw_screen_implicit(a, b, c) 
#define set_argument(a, b, c, d, e, f, g) plib_set_arg(a, b, c, d, e, f, g)

int main(int system_argument_amount, char *system_argument_array[]){
	// Set arguments
	struct plib_argument args[3] = {0};
	set_argument("--help","show this dialog","void",NULL,help_callback, 0,args);
	set_argument("--auto","run the program with example data", "void", NULL,auto_callback,0,args);
	set_argument("--verbose","show extra information","void",NULL,verbose_callback,0,args);
	quit_handler();

	if(plib_proccess_arguments(args) == 0){ // Arguments parsed successfully
		
		// Define the order list
		int orders_capacity        = 1,
				order_index            = 0;

		Burrito *order_list = malloc(orders_capacity * sizeof(Burrito));
		if(!order_list){
			printf("Error!! memory leak occured!!! the burritos are free!!!!\n");
			return 1;
		}
		
		// Start the input loop
		while(1){
			char *screen_main[]={"Enter Order","Management Summary","Kitchen Screen","Exit"};
			int screen_main_value = draw_screen(screen_main,4,"-- Select An Option --");
			switch (screen_main_value){
				case 0: // Enter order
					re_order:

					// Dynamically allocate more memory
					if(order_index == orders_capacity){
						if(verbose){
							printf("orders struct has been DMA'D from %d -> %d.\n",orders_capacity,orders_capacity*2);
							sleep(USER_SLEEP_DELAY);
						}

						orders_capacity *= 2;
						Burrito *temp = realloc(order_list,orders_capacity * sizeof(Burrito));
						if(!temp){
							if(verbose) printf("error with tmp size\n");
							free(order_list);
							return 1;
						} else order_list = temp;
					}

					// initiate the burrito order ( singular )
					Burrito order = order_list[order_index];

					// get delivery options
					char *screen_location[]={"Pickup","Delivery","Dine In",};
					int screen_location_value = draw_screen(screen_location,3,"Is your order for pickup or delivery?");
					order.mode_val = screen_location_value;
					order.mode = screen_location[screen_location_value];
					if(screen_location_value==1){
						printf("%sA %s%0.2f\033[0m%s delivery charge has been added to your order.\033[0m\n",DIS_ANSI,BOLD_ANSI,DELIVERY_CHARGE,DIS_ANSI);
						order.price += DELIVERY_CHARGE;
						sleep(USER_SLEEP_DELAY);	
					}

					// get other basic input info 
					order.name = input_checker("Please enter your","name");
					order.number = input_checker("Please enter your","number");

					// only ask for address if mode is delivery	
					if(screen_location_value) order.address = input_checker("Please enter your","address");

					// allocate memory for type(s);
					order.type_capacity = 1;
					order.type_index = 0;
					order.type = malloc(order.type_capacity * BURRITO_ALLOC_SIZE);

					while(1){
						int user_type_input = 1;
						if(user_type_input<=2) order.price += BURRITO_CHEAP_PRICE;
						else order.price += BURRITO_EXPENSIVE_PRICE;	
						// dynmically allocate more size 
						if(order.type_index == order.type_capacity){
							order.type_capacity += 1;
							if(verbose){
								printf("Allocated %lu bytes (%lu bytes total) for more burritos!\n",sizeof(BURRITO_TYPE_LIST[user_type_input]),sizeof(order.type));
								sleep(USER_SLEEP_DELAY);
							}
							char** temp = realloc(order.type,sizeof(order.type)+sizeof(BURRITO_TYPE_LIST[user_type_input]));
							if(!temp){
								printf("Memory allocation failed in type.\n");
								free(order.type);
								return 1;
							}
							order.type = temp;
						}
						order.type[order.type_index] = (char *)BURRITO_TYPE_LIST[user_type_input];
						order.type_index++;
					}
					char *screen_complete_order[]={"Yes","No, Restart Order",};
					char price_header[256];
					snprintf(price_header,sizeof(price_header),"Final Price: %s$%0.2f\033[0m, Confirm Order?",BOLD_ANSI,order.price);
					if(!draw_screen(screen_complete_order,2,price_header)){
						// finished
						printf("User ordered burritos:\n");
						for(int i=0;i<order.type_index;i++){
							printf("%s\n",order.type[i]);
						}
						sleep(5);
						order_index++;
						order_amount_global=order_index;
					} else {
						printf("Restarting order.\n");
						sleep(USER_SLEEP_DELAY);
						goto re_order;
					}
					break;
				case 1: // Management Summary
					if(order_index>0) print_management_values(order_list,order_index);
					else printf("No orders to view\n");
					achar();
					break;
				case 2: // Kitchen Screen 
					break;

				case 3: // exit the program
					free(order_list);
					return 0;
			}
		}
	}
	return 0;
}
