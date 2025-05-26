#ifndef MAIN_LIB_H        // guard so that main_lib cant be used twice
#define MAIN_LIB_H

extern int help_toggle;
extern int verbose;
extern int exit_verbose;


void help_callback(void);
void verbose_callback(void);
void exit_verbose_callback(void);

extern int order_amount_global;
extern float order_price_global;

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

// COLORS 
#define BOLD_ANSI "\033[1m"
#define SEL_ANSI  "\033[0;38;2;32;160;153;49m"
#define DIS_ANSI  "\033[0;38;2;68;66;122;49m"
#define clear_a() printf("\033[0;0H")

// PRICES
#define BURRITO_CHEAP_PRICE      8.5
#define BURRITO_EXPENSIVE_PRICE  13.5
#define DELIVERY_CHARGE          2.5
#define BURRITO_PRICE_CHANGE_NUM 2


// MISC VALUES 
#define BURRITO_LIMIT       90 // per burrito so 90*6
#define USER_SLEEP_DELAY    1
#define BURRITO_TYPE_AMOUNT 6


// INPUT BOUNDS
#define INPUT_MIN_NAME    2 
#define INPUT_MAX_NAME    32
#define INPUT_MIN_NUMBER  8 
#define INPUT_MAX_NUMBER  16
#define INPUT_MIN_ADDRESS 8
#define INPUT_MAX_ADDRESS 128

void draw_header(void); 
void draw_header_sep(void);
void quit(int a);
void handle_quit(void);
Burrito_type *display_burrito_menu(void);
int draw_kitchen_screen(Burrito *order_list, int order_index);
void draw_mangement(Burrito *order_list, int order_index);
int achar(void);

// GLOBAL VARS
extern const char *BURRITO_TYPE_LIST[];
extern int orders_capacity;
extern int order_index;

#ifdef _WIN32
	#define clear()  system("cls");
#elif __unix__
	#define clear()  system("clear");
#endif

struct plib_argument {
	char *arg;
	char *desc;
	char *type;
	char *value;
	void (*call)(const char *value);
	void (*func)(void);
	char (*str_call)(const char *value);
	int  (*int_call)(const char *value);
	int scall;
	// ^ self callback ( calls function with its own pointer );
};

int proccess_arguments(int argc, char*argv[],struct plib_argument *local);
int set_argument(char* argument, char* description, char* type,void (*callback)(const char *value), void (*function)(void),int self_callback,struct plib_argument *local, int  max_length);
#endif // MAIN_LIB_H
