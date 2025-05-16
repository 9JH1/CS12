#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "plib.c"
#include "main_settings.c"

#ifdef _WIN32
	#include <conio.h>
	int achar() {
		if (_kbhit()) {
			return _getch();
		}
		return -1;
	}

#elif __unix__
	#include <termios.h>
	int achar_unix(){
		static struct termios oldt, newt;
		tcgetattr(STDIN_FILENO,&oldt);
		newt=oldt;
		newt.c_lflag &= ~(ICANON | ECHO );
		tcsetattr(STDIN_FILENO,TCSANOW,&newt);
		int ch=getchar();
		tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
		return ch;
	}
#endif

#define achar() achar_unix()
#define clear()  system("clear");
#define clear_a() printf("\033[H");

// define callback toggles
int help_toggle = 0;
int verbose     = 0;

// define callback functions
void help_callback(){help_toggle = 1;}
void verbose_callback(){verbose = 1;}

int order_amount_global=0;
static float order_price_global = 0;

// datatypes
typedef enum {
    PICKUP,
    DELIVERY,
    DINEIN
} OrderMode;

typedef struct Burrito_type{
	char *type;
	int amount;
	float price;
}Burrito_type;

typedef struct Burrito {
	OrderMode mode;
	char*name;
	char*number;
	char*address;
	float price;
	Burrito_type *type;
}Burrito; 





void draw_header_sep_implicit(){
	printf("%s----------------------------\033[0m\n",DIS_ANSI);
}
void draw_header_implicit(){
printf("Welcome To %sBanjo's Burritos\033[0m!%s Order #%d ($%0.2f)\033[0m\n",BOLD_ANSI,DIS_ANSI,order_amount_global+1,order_price_global);
	draw_header_sep_implicit();
}

// all this does is run when the user presses control+c
void quit(int a){
	printf("\nCTRL+C pressed have fun with your leaked memory, Goodbye\n");
	exit(0);
}

void handle_quit_implicit(){
	signal(SIGINT,quit);
}

Burrito_type *display_burrito_menu(){
	int burrito_list_malloc = 0;
	/*for(int i=0;i<BURRITO_TYPE_AMOUNT;i++){
		burrito_list_malloc+=sizeof(BURRITO_TYPE_LIST[i]);
	}*/ 
	burrito_list_malloc = sizeof(Burrito)*BURRITO_TYPE_AMOUNT;
	printf("%d\n",burrito_list_malloc);
	Burrito_type *burrito_list = malloc(burrito_list_malloc);
	for(int i=0;i<BURRITO_TYPE_AMOUNT;i++){
		burrito_list[i].type = (char *)BURRITO_TYPE_LIST[i];
	}
	for(int i=0;i<BURRITO_TYPE_AMOUNT;i++){
		burrito_list[i].price = (i<3) ? BURRITO_CHEAP_PRICE : BURRITO_EXPENSIVE_PRICE;
		burrito_list[i].amount = 0;
	}
	
	// draw ui 
	int largest_name = 0;
	for(int i=0;i<BURRITO_TYPE_AMOUNT;i++){
		char*  cur_name  = burrito_list[i].type;
		if(largest_name<strlen(cur_name)) largest_name = strlen(cur_name);
	}
	
	// draw frame
	int selected_line_index=0;
	clear();
	while(1){
		int largest_amount=0;
		float price_sum=0;
		int  amount_sum=0;
		clear_a();
		draw_header_implicit();
		for(int i=0;i<BURRITO_TYPE_AMOUNT;i++){
			char largest_char[16];
			snprintf(largest_char,sizeof(largest_char),"%d",burrito_list[i].amount);
			if(largest_amount<strlen(largest_char)) largest_amount = strlen(largest_char);
			price_sum+=burrito_list[i].amount*burrito_list[i].price;
			amount_sum+=burrito_list[i].amount;
		}
		for(int i=0;i<(BURRITO_TYPE_AMOUNT+1);i++){
			if(selected_line_index==i){
				// write sel panel 
				printf("%s%s> ",BOLD_ANSI,SEL_ANSI);
			} else printf("  ");

			if(i==0){
				printf("Reset\033[0m\n");
				continue;
			}
			Burrito_type local = burrito_list[i-1];
		
			char cur_amount[16]="";
			char cur_price[16]="";
			sprintf(cur_amount,"%d",local.amount);
			sprintf(cur_price,"%1.2f",local.price);
			printf("%s",cur_amount);
			if(largest_amount>=1) for(int j=0;j<largest_amount-strlen(cur_amount);j++) printf(" ");
			printf(" | %s",local.type);
			if(largest_name>=1) for(int j=0;j<largest_name-strlen(local.type);j++) printf(" ");

			printf(" |%s\033[0m",cur_price);
			if(strlen(cur_price)>=1) for(int j=0;j<strlen(cur_price);j++) printf(" ");
			printf("\n");
		}
		draw_header_sep_implicit();
		printf("price: %s$%0.2f\033[0m %s(%d burritos)\033[0m",BOLD_ANSI,price_sum,DIS_ANSI,amount_sum);
		char price_sum_char[64];
		sprintf(price_sum_char,"%0.2f",BURRITO_TYPE_AMOUNT*(BURRITO_LIMIT*BURRITO_EXPENSIVE_PRICE));
		
		char amount_sum_char[64];
		sprintf(price_sum_char,"%d",BURRITO_TYPE_AMOUNT*BURRITO_LIMIT);
		for(int i=0;i<strlen(amount_sum_char)+strlen(price_sum_char);i++) printf(" ");
		printf("\n");
		int ch=achar(); 
		switch(ch){
			case 'B':
				// down
				if(selected_line_index==BURRITO_TYPE_AMOUNT) selected_line_index = 0;
				else selected_line_index++;
				break;
			case 'A':
				if(selected_line_index==0) selected_line_index=BURRITO_TYPE_AMOUNT;
				else selected_line_index--;
				// up
				break;
			case 'C':
				// right
				if(selected_line_index!=0 && burrito_list[selected_line_index-1].amount < BURRITO_LIMIT) burrito_list[selected_line_index-1].amount++;
				break;
			case 'D':
				// left
				if(selected_line_index!=0 && burrito_list[selected_line_index-1].amount > 0) burrito_list[selected_line_index-1].amount--;
				break;
			case 10:
				// enter
				if(selected_line_index == 0){
					for(int i=0;i<BURRITO_TYPE_AMOUNT;i++){
						burrito_list[i].amount = 0;
					}
				} else {
					order_price_global+=price_sum;
					printf("%f\n",price_sum);
					return burrito_list;
				}
				break;
		}
	}
}
