/*  THIS FILE IS AN EXTENSION OF FILE main.c 
 *  ALL LICENSING AND RULES ALSO INCLUDE THIS
 *  FILE AND ALL CONNECTED FILES.
 */

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include "plib.c"

#define clear()  system("clear");
#define clear_a() printf("\033[H");
#define BOLD_ANSI "\033[1m"

#define AUTO_SLEEP_DELAY 1
#define USER_SLEEP_DELAY 2

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


int achar(){
	static struct termios oldt, newt;
	tcgetattr(STDIN_FILENO,&oldt);
	newt=oldt;
	newt.c_lflag &= ~(ICANON | ECHO );
	tcsetattr(STDIN_FILENO,TCSANOW,&newt);
	int ch=getchar();
	tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
	return ch;
}

int draw_screen(char *array[],size_t array_length,char *prompt){
	int selected=0;
	if(auto_toggle) selected = nrand(0,array_length);
	clear();
	while (1){
		clear_a();
		printf("%s",prompt);
		for(int i=0;i<array_length;i++){
			if(selected == i){
				char code[16];
				color("#00ff00","#ff0000",code);
				printf("%s>",code);
			} else printf(" ");
			printf(" %s\033[0m\n",array[i]);
		}
		if(auto_toggle) {
			sleep(AUTO_SLEEP_DELAY);
			return selected;
		}
		int ch = achar();
		if (ch == 'B'){
			if(selected==array_length-1) selected=0;
			else selected++;
		} else if (ch == 'A'){
			if(selected==0) selected=array_length-1;
			else selected--;
		} else if (ch == 10) return selected;
	}
	return -1;
}
