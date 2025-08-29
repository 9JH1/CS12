// Librarys
#include "input.h"
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Base Variables
InputEvent InputEventPipe;                       // global input object
int framecount = 0;                              // frame count ( update adder )
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER; // mutex for variable saftey
pthread_t input_pipe;                            // pthread pipe object

// Data types
typedef struct {
  const char *text;
  const char *hover_text;
  const char *ansi;
  char *hover_ansi;
  int x;
  int y;
  InputEvent *update_callback;
} button_opt;

// Function declarations
void *button(void *arg);         // display interactive button
void *input_listener(void *arg); // update InputEventPipe in realtime
void quit(int code);             // handle quit

int main(void) {
  signal(SIGINT, quit);
  pthread_create(&input_pipe, NULL, input_listener, NULL);

  const button_opt settings_1 = {
      .text = "Hello World!",
      .hover_text = "testtesttest",
      .ansi = "",
      .hover_ansi = "\033[39;42m",
      .x = 10,
      .y = 10,
      .update_callback = &InputEventPipe,
  };

  const button_opt settings_2 = {
      .text = "testtesttest",
      .hover_text = "Hello World!",
      .ansi = "",
      .hover_ansi = "\033[39;42m",
      .x = 10,
      .y = 11,
      .update_callback = &InputEventPipe,
  };

  pthread_t button_1;
  pthread_create(&button_1, NULL, button, (void *)&settings_1);

  button((void *)&settings_2);
  quit(0);
}

void *input_listener(void *arg) {
  while (1) {
    InputEvent local;
    input(&local);
    pthread_mutex_lock(&mut);
    framecount++;
    InputEventPipe = local;
    pthread_mutex_unlock(&mut);
  }
  return NULL;
}

void quit(int code) {
  printf("\rProgram Exiting..\n");
  disable_raw_mode();
  disable_mouse_reporting();
  pthread_cancel(input_pipe);
  pthread_join(input_pipe, NULL);
  pthread_mutex_destroy(&mut);
  pthread_exit(0);
  exit(code);
}

void *button(void *arg) {
  button_opt *opt = (button_opt *)arg;
  int ext = strlen(opt->text) - strlen(opt->hover_text);
  int pos_x = -1;
  int pos_y = -1;
  int type = -1;

  // start button loop
  while (1) {
    const InputEvent local = *opt->update_callback;
    if (local.type != 0 && local.type != 1)
      continue;

    // await frame update
    while (1) {
      const int new_x = opt->update_callback->data.mouse.x;
      const int new_y = opt->update_callback->data.mouse.y;
      const int new_type = opt->update_callback->type;
      if (pos_x != new_x || pos_y != new_y || type != new_type) {
        pos_x = new_x;
        pos_y = new_y;
        type = new_type;
        break;
      }
      usleep(100);
    }

    printf("\033[1;1H");
    if (pos_y != opt->y || pos_x < opt->x ||
        pos_x >= opt->x + strlen(opt->text)) {

      printf("\033[%d;%dH%s%s\033[0m\033[K", opt->y, opt->x, opt->ansi,
             opt->text);

      continue;
    }

    // print hover text
    printf("\033[%d;%dH%s%s", opt->y, opt->y, opt->hover_ansi, opt->hover_text);

    if (ext > 0)
      for (int i = 0; i > ext + 1; i++)
        printf(" ");

    printf("\033[0m\033[K");

    // check if mouse was a click or not;
    if (opt->update_callback->type == 1) {
      return NULL;
    }
  }
}
