#include "../lib/ansi.h"
#include "../lib/draw.h"
#include "../lib/input.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void quit(int code) {
  deactivate_terminal_buffer();
  show_cursor();
  exit(code);
}

typedef struct {
  solid_opt *par;
  char *string;
  int index;
  const char **text;
  const int text_len;
} text_opt;

void text(text_opt opt) {
  // add string val => reduce printf calls
  for (int i = opt.index; i < opt.text_len; i++) {
    const int local_index = i - opt.index;
    const int max_width = opt.par->width - 2;
    const int max_height = opt.par->height - 3;
    const int current_y = opt.par->y + local_index;
    const int current_x = opt.par->x;

    gotoxy(current_x, current_y);
    if (local_index > max_height)
      break; // overflow y

    if (strlen(opt.text[i]) > max_width) {
      for (int j = 0; j < max_width; j++) {
        gotoxy(current_x + j, current_y);
        printf("%c", opt.text[i][j]);
      }
    } else
      printf("%s", opt.text[i]);
  }
}

int main(const int argc, const char *argv[]) {
  activate_terminal_buffer();
  hide_cursor();
  clear();

  signal(SIGINT, quit);
  printf("Show System Arguments\nExit\n");

  while (1) {
    // await button click
    InputEvent loc;
    const int ret = input(&loc);
    if (loc.type == MOUSE_CLICK) {
      if (loc.data.mouse.m == 0) {
        if (loc.data.mouse.y == 1)
          break;
        else if (loc.data.mouse.y == 2)
          quit(1);
      }
    }
  }

  const termsize local = get_term_size();
  const int box_width = local.WIDTH / 1.5;
  const int box_height = local.HEIGHT / 1.5;
  const int box_x = (local.WIDTH / 2) - (box_width / 2);
  const int box_y = (local.HEIGHT / 2) - (box_height / 2);

  box_opt settings = {.border =
                          (border_opt){
                              .bottom = "━",
                              .bottom_left_corner = "┗",
                              .bottom_right_corner = "┛",
                              .top_left_corner = "┏",
                              .top_right_corner = "┓",
                              .top = "━",
                              .left = "┃",
                              .right = "┃",
                          },
                      .solid =
                          (solid_opt){
                              .width = box_width,
                              .height = box_height,
                              .x = box_x,
                              .y = box_y,
                              .fill = ' ',
                              .ansi = "\033[0m",
                          },
                      .title = (title_opt){
                          .title = "System Arguments: ",
                          .prefix = "┫",
                          .suffix = "┣",
                          .padding = 1,
                          .pos = TOP_LEFT,
                      }};
  box(&settings);

  // draw text
  int index = 0;
  while (1) {
    clear();

    // needa convert this into a function :D
    printf("%s\n", settings.string);
    text((text_opt){
        .par = &settings.solid,
        .index = index,
        .text_len = argc,
        .text = argv,
    });

    gotoxy(1, 1);
    printf("MOUSE_SCROLL_Y: %d", MOUSE_SCROLL_Y);
    fflush(stdout);

    index = await_scroll();
    if (index <= 0) {
      index = 1;
    } else if (index > argc) {
      index = argc - 1;
    }
  }

  free_box_opt(&settings);
  return 0;
}
