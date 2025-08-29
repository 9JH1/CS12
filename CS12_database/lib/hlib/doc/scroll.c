#include "../lib/ansi.h"
#include "../lib/input.h"
#include <stdio.h>

// signal
#include <signal.h>
#include <stdlib.h>

void quit(int code) {
  deactivate_terminal_buffer();
  exit(1);
}

int main(const int argc, const char *argv[]) {
  signal(SIGINT, quit);
  activate_terminal_buffer();
  int index = 0;

  while (1) {
    clear_screen();
    gotoxy(0, 0);

    // draw file
    for (int i = index; i < argc; i++) {
      const char *c = argv[i];
      if (c)
        printf("%s\n", c);
    }
    printf("%d\n", MOUSE_SCROLL_Y);

    index = await_scroll();
    if (index < 0)
      index = 0; // min bound
    if (index > argc)
      index = argc; // max bound
  }
}
