#include "lib/hlib/input.h"
#include <pthread.h> 
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
InputEvent event;
int event_update = 0;
bool keep_alive = true;


// await a new input
int input_pipe(void *arg){
	while(keep_alive){
		int ret = input(&event);
		event_update++;
	}
	return 1;
}

void ui_quit_handle(int code){
	exit(code);
}

int main(void){
	signal(SIGINT,ui_quit_handle);

	pthread_t input_pipe_t;
	pthread_create(&input_pipe_t,NULL,NULL,input_pipe);
	
	int old_frame;
	while(keep_alive){
		old_frame = event_update;
		while(old_frame == event_update){
			usleep(50);
		}
	}
	return 0;
}
