#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <bsd/string.h>
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
int auto_toggle = 0;
int help_toggle = 0;
int verbose     = 0;

// define callback functions
void auto_callback(){auto_toggle = 1;}
void help_callback(){help_toggle = 1;}
void verbose_callback(){verbose = 1;}

int nrand(int min, int max) {
  srand(time(NULL));
  int range = max - min;
  return rand() % range + min;
}
int order_amount_global=0;

typedef struct Burrito {
	char*type;
	char*mode; // 0 = delivery, 1 = pickup
	int mode_val;
	char*name;
	char*number;
	char*address;
	float price;
}Burrito;

// all this does is run when the user presses control+c
void quit(int a){printf("\nCTRL+C pressed have fun with your leaked memory, Goodbye\n");exit(0);}





void draw_recept_sep(){
	printf("%s------------ORDER-----------\033[0m\n",DIS_ANSI);
}
void draw_header_sep(){
	printf("%s----------------------------\033[0m\n",DIS_ANSI);
}
void draw_header(){
	printf("Welcome To %sBanjo's Burritos\033[0m!%s Order #%d\033[0m\n",BOLD_ANSI,DIS_ANSI,order_amount_global+1);
	draw_header_sep();
}
void draw_order_header(){
	printf("Welcome To %sBanjo's Burritos\033[0m!\n",BOLD_ANSI);
	draw_recept_sep();
}



int draw_screen_implicit(char *array[],size_t array_length,char *prompt){
	int selected=0;
	if(auto_toggle) selected = nrand(0,array_length);
	clear();
	while (1){
		clear_a();
		draw_header();
		printf("%s\n",prompt);
		for(int i=0;i<array_length;i++){
			if(selected == i){
				printf("%s%s>",SEL_ANSI,BOLD_ANSI);
			} else printf(" ");
			printf(" %s\033[0m\n",array[i]);
		}
		if(auto_toggle){
			break; 
	} else {
			int ch = achar();
			if (ch == 'B'){
				if(selected==array_length-1) selected=0;
				else selected++;
			} else if (ch == 'A'){
				if(selected==0) selected=array_length-1;
				else selected--;
			} else if (ch == 10) break;
		}
	}
	draw_header_sep();
	printf("\nYou selected %s%s\033[0m\n",BOLD_ANSI,array[selected]);
	fflush(stdout);
	sleep(USER_SLEEP_DELAY);
	return selected;
}


char **create_label_price(const char *labels[], size_t label_amount) {
    char **buffer = (char **)malloc(label_amount * sizeof(char *));
    if (buffer == NULL) return NULL;

    size_t largest = 0;
    for (size_t i = 0; i < label_amount; i++) {
        size_t len = strlen(labels[i]);
        if (len > largest) largest = len;
    }

    for (size_t i = 0; i < label_amount; i++) {
        double price = (i > BURRITO_PRICE_CHANGE_NUM) ? BURRITO_EXPENSIVE_PRICE : BURRITO_CHEAP_PRICE;
        size_t buffer_size = largest + 20;
        buffer[i] = (char *)malloc(buffer_size);
        if (buffer[i] == NULL) {
            for (size_t j = 0; j < i; j++) free(buffer[j]);
            free(buffer);
            return NULL;
        }
        snprintf(buffer[i], buffer_size, "%-*s $%.2f", (int)largest, labels[i], price);
    }

    return buffer;
}


void draw_border(char*text,int size){
	printf("|%s",text);
	if(size>0 && strlen(text)>0){
		for(int i=0;i<size-2-strlen(text);i++){
			printf(" ");
		}
		printf("|\n");
	}
}

void free_labels(char **buffer, size_t label_amount) {
	for (size_t i = 0; i < label_amount; i++) {
  	free(buffer[i]);
  }
  free(buffer);
}

void seperator(char *c,int a){printf("\n");for(int i=0;i<a;i++) printf("%s",c);printf("\n\n");}
char* input_checker(char *prompt,char *keyword){
	while(1){
		clear();
		draw_header();	
		printf("%s %s:%s ",prompt,keyword,BOLD_ANSI);
		static char input[256]="";
		fgets(input,sizeof(input),stdin);
		printf("\033[0m");
		if(strcmp(input,"\n")!=0 && strlen(input)>0){
			return input;
		} else {
			printf("%sPlease Enter your %sACTUAL %s\033[0m%s.\033[0m\n",DIS_ANSI,BOLD_ANSI,keyword,DIS_ANSI);
			sleep(USER_SLEEP_DELAY);
		}
	}
}

void print_management_values(Burrito *orders, int order_index){	
	float total_sales=0;
	int pickup=0;
	int delivery=0;
	int eat_in=0;

	// calculate sums 
	for(int i=0;i<order_index;i++){
		const Burrito order = orders[i];

		if(order.mode_val == 0) pickup++;
		else if (order.mode_val == 1) delivery++;
		else if (order.mode_val == 2) eat_in++;				
		total_sales+=order.price;
		printf("%f",order.price);
	}
	draw_header_sep();
	printf("There have been %s%d\033[0m orders placed, costing %s$%0.2f\033[0m\n",BOLD_ANSI,order_index,BOLD_ANSI,total_sales);
	printf("%s%d\033[0m Customer(s) picked up their burrito, %s%d\033[0m Customer(s)  had it delivered and %s%d\033[0m Customer(s) ate inside the resteraunt.\n",BOLD_ANSI,pickup,BOLD_ANSI,delivery, BOLD_ANSI,eat_in);
	draw_header_sep();
	printf("%sPress %sany key\033[0m%s to go back to main menu.\033[0m\n",DIS_ANSI,BOLD_ANSI,DIS_ANSI);
}

void quit_handler(){
	signal(SIGINT,quit);
}
