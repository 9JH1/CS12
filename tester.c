#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "main_lib.c"

typedef struct  Burritos{
	char *type;
	float price;
	int amount;
}Burritos;

#define BURRITO_AMOUNT 6
#define BURRITO_BASE_SIZE 16
#define BURRITO_ALLOC (BURRITO_AMOUNT * BURRITO_BASE_SIZE)
#define BURRITO_CHEAP_PRICE 8.5
#define BURRITO_EXPENSIVE_PRICE 13.5
#define BURRITO_LIMIT 90
#define BURRITO_SINGLE_LIMIT 9 // the 1 is the limit


// draw_burrito_menu ()


int main(){

	// create burrito list
	Burritos *burrito_list = malloc(BURRITO_ALLOC);
	burrito_list[0].type = "Cheese";
	burrito_list[1].type = "Plain";	
	burrito_list[2].type = "Spicy";
	burrito_list[3].type = "Deluxe";
	burrito_list[4].type = "Large";
	burrito_list[5].type = "Gorumet";
	for(int i=0;i<BURRITO_AMOUNT;i++){
		burrito_list[i].price = (i<3) ? BURRITO_CHEAP_PRICE : BURRITO_EXPENSIVE_PRICE;
		burrito_list[i].amount = 0;
	}
	
	// draw ui 
	int largest_name = 0;
	int color_val = 0;
	for(int i=0;i<BURRITO_AMOUNT;i++){
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
		for(int i=0;i<BURRITO_AMOUNT;i++){
			char largest_char[16];
			snprintf(largest_char,sizeof(largest_char),"%d",burrito_list[i].amount);
			if(largest_amount<strlen(largest_char)) largest_amount = strlen(largest_char);
			price_sum+=burrito_list[i].amount*burrito_list[i].price;
			amount_sum+=burrito_list[i].amount;
		}
		for(int i=0;i<(BURRITO_AMOUNT+1);i++){

			if(selected_line_index==i){
				// write sel panel 
				printf(">");
			} else printf(" ");

			if(i==0){
				printf("Reset\n");
				continue;
			}
			Burritos local = burrito_list[i-1];
		
			char cur_amount[16]="";
			char cur_price[16]="";
			snprintf(cur_amount,sizeof(cur_amount),"%d",burrito_list[i].amount);
			snprintf(cur_price,sizeof(cur_price),"%2.2f",burrito_list[i].price);

			printf("<%s>",cur_amount);
			for(int j=0;j<largest_amount-strlen(cur_amount);j++) printf(" ");
			printf(" | %s",local.type);
			for(int j=0;j<largest_name-strlen(local.type);j++) printf(" ");
			printf(" | %s\n",cur_price);
		}
		printf("price: $%0.2f (%d burritos)                 \n",price_sum,amount_sum);
		int ch=achar();
		switch(ch){
			case 'B':
				// down
				if(selected_line_index==BURRITO_AMOUNT) selected_line_index = 0;
				else selected_line_index++;
				break;
			case 'A':
				if(selected_line_index==0) selected_line_index=BURRITO_AMOUNT;
				else selected_line_index--;
				// up
				break;
			case 'C':
				// right
				if(burrito_list[selected_line_index].amount < BURRITO_LIMIT) burrito_list[selected_line_index].amount++;
				break;
			case 'D':
				// left
				if(burrito_list[selected_line_index].amount > 0) burrito_list[selected_line_index].amount--;
				break;
			case 10:
				// enter
				if(selected_line_index == 0){
					for(int i=0;i<BURRITO_AMOUNT;i++){
						burrito_list[i].amount = 0;
					}
				} else return selected_line_index-1;
				break;
		}

	}

	// return user_input;
	free(burrito_list);
	return 0;
}
