#include "thrd.h"
#include "uilib.h"
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

bool noquit = true;
pthread_mutex_t mut;

void quit(int code) {
  noquit = false;
  disable_mouse_reporting();
  pthread_mutex_destroy(&mut);
  ka_thrd();
  deactivate_terminal_buffer();
  pthread_exit(0);
  exit(code);
}

int frame;
int selected = 0;

// increment frame each 1 second
void *update_pipe(void *in) {
  while (noquit) {
    frame++;
    sleep(1);
  }
  return NULL;
}

// this is the main input thread, this is where all
void *input_pipe(void *in) {
  while (noquit) {
    pthread_mutex_lock(&mut);
    mouse ret_m;
    char ret_s;
    const int ret = await_click(&ret_m, &ret_s);
    pthread_mutex_unlock(&mut);

    // check if return is string or mouse
    if (ret == 0) {
      // return type was mouse
      if (ret_m.m == 0) {
        // right click was pressed
        pthread_mutex_lock(&mut);
        selected = ret_m.y;
        frame++;
        pthread_mutex_unlock(&mut);
      }
    } else if (ret == 1) {
      // return type was string
      pthread_mutex_lock(&mut);
      switch (ret_s) {
      case 'a':
        selected = 1;
        break;

      case 'b':
        selected = 0;
        break;
      }
      frame++;
      pthread_mutex_unlock(&mut);
    } else {
      // function error
    }
  }
  return NULL;
}

int main() {
  signal(SIGINT, quit);
  activate_terminal_buffer();

  pthread_mutex_init(&mut, NULL);
  cs_thrd(input_pipe);
  cs_thrd(update_pipe);

  // graphics pipe
  int old_frame = -1;
  while (noquit) {
    while (frame == old_frame) {
      usleep(50);
    }

    // draw frame
    system("clear");
    old_frame = frame;
    if (selected == 0) {
      printf("> ONE\n  TWO\n");
    } else {
      printf("  ONE\n> TWO\n");
    }
    printf("%d\n", frame);
  }
}
