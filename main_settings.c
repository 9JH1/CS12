
// COLORS 
#define BOLD_ANSI "\033[1m"
#define SEL_ANSI  "\033[32m"
#define DIS_ANSI  "\033[31m"


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


// GLOBAL VARS
const char *BURRITO_TYPE_LIST[]={"Cheese","Plain","Spicy","Deluxe","Large","Gourmet"};
int orders_capacity=1;
int order_index=0;

