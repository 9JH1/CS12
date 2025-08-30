#include "lib/uilib.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

termsize await_resize(void);
typedef struct {
  int height;
  int width;
  int x;
  int y;
  char *background;
  char *foreground;
} sprite_opt;

typedef struct {
  int width;
  int height;
  int x;
  int y;

  char *top;
  char *top_left_corner;
  char *top_right_corner;

  char *bottom;
  char *bottom_left_corner;
  char *bottom_right_corner;

  char *left;
  char *right;
  char *top_left_title_border;
  char *top_right_title_border;
  char *bottom_left_title_border;
  char *bottom_right_title_border;

  char *ansi;
  char *top_left_title_ansi;
  char *top_right_title_ansi;
  char *bottom_left_title_ansi;
  char *bottom_right_title_ansi;

  char *top_left_title;
  char *top_right_title;
  char *bottom_left_title;
  char *bottom_right_title;
  int title_padding;
  char *fill_char;
} box;

int draw_box(box o) {

  // allocate matrix for box
  char *box_string[o.height][o.width];

  // fill the space
  for (int i = 1; i < o.height - 1; i++) {
    for (int j = 1; j < o.width - 1; j++) {
      box_string[i][j] = o.fill_char;
    }
  }

  // corners
  box_string[0][0] = o.top_left_corner;
  box_string[0][o.width - 1] = o.top_right_corner;
  box_string[o.height - 1][0] = o.bottom_left_corner;
  box_string[o.height - 1][o.width - 1] = o.bottom_right_corner;

  for (int i = 1; i < o.width - 1; i++) {
    box_string[0][i] = o.top;
    box_string[o.height - 1][i] = o.bottom;
  }

  // left and right
  for (int i = 1; i < o.height - 1; i++) {
    box_string[i][0] = o.left;
    box_string[i][o.width - 1] = o.right;
  }

  // now that the borders and fill is done, we now place the titles
  // top-left-title
  if (strcmp(o.top_left_title, "") != 0) {
    box_string[0][1 + o.title_padding] = o.top_left_title_border;

    box_string[0][2 + strlen(o.top_left_title) + o.title_padding] =
        o.top_right_title_border;
  }

  // top-right-title
  if (strcmp(o.top_right_title, "") != 0) {
    box_string[0][o.width - 2 - o.title_padding] = o.top_right_title_border;
    box_string[0][o.width - 3 - o.title_padding - strlen(o.top_right_title)] =
        o.top_left_title_border;
  }

  // bottom-left-title
  if (strcmp(o.bottom_left_title, "") != 0) {
    box_string[o.height - 1][1 + o.title_padding] = o.bottom_left_title_border;

    box_string[o.height - 1][2 + strlen(o.bottom_left_title) +
                             o.title_padding] = o.bottom_right_title_border;
  }

  // bottom-right-title
  if (strcmp(o.bottom_right_title, "") != 0) {
    box_string[o.height - 1][o.width - 2 - o.title_padding] =
        o.bottom_right_title_border;

    box_string[o.height - 1]
              [o.width - 3 - o.title_padding - strlen(o.bottom_right_title)] =
                  o.bottom_left_title_border;
  }

  // actual draw
  for (int i = 0; i < o.height; i++) {
    for (int j = 0; j < o.width; j++) {
      gotoxy(o.x + j, o.y + i);
      printf("%s%s\033[0m", o.ansi, box_string[i][j]);
    }
  }

  // draw titles
  if (strcmp(o.top_left_title, "") != 0) {
    gotoxy(o.x + 2 + o.title_padding, o.y);
    printf("%s%s\033[0m",
           (strcmp(o.top_left_title_ansi, "") != 0) ? o.top_left_title_ansi
                                                    : o.ansi,
           o.top_left_title);
  }

  if (strcmp(o.top_right_title, "") != 0) {
    gotoxy(o.x + o.width - o.title_padding - 2 - (int)strlen(o.top_right_title),
           o.y);
    printf("%s%s\033[0m",
           (strcmp(o.top_right_title_ansi, "") != 0) ? o.top_right_title_ansi
                                                     : o.ansi,
           o.top_right_title);
  }

  if (strcmp(o.bottom_left_title, "") != 0) {
    gotoxy(o.x + 2 + o.title_padding, o.y + o.height - 1);
    printf("%s%s\033[0m",
           (strcmp(o.bottom_left_title_ansi, "") != 0)
               ? o.bottom_left_title_ansi
               : o.ansi,
           o.bottom_left_title);
  }

  if (strcmp(o.bottom_right_title, "") != 0) {
    gotoxy(o.x + o.width - o.title_padding - 2 -
               (int)strlen(o.bottom_right_title),
           o.y + o.height - 1);
    printf("%s%s\033[0m",
           (strcmp(o.bottom_right_title_ansi, "") != 0)
               ? o.bottom_right_title_ansi
               : o.ansi,
           o.bottom_right_title);
  }

  gotoxy(o.width + o.x - 1, o.height + o.y - 1);
  printf("\033[0m"); // just in case ;3
  return 0;
}

int draw_sprite(const sprite_opt loc, const char *sprite[]) {
  if (loc.y + loc.height > get_term_size().COL) {
    // printf("Sprite to large for terminal\n");
    return -1;
  }

  char base_ansi[64];
  hex2ansi_buf(loc.foreground, loc.background, base_ansi, sizeof base_ansi);

  for (int y = 0; y < loc.height; y++) {
    gotoxy(loc.x, loc.y + y);
    // ^ set co-ordinate base

    // print row by row
    printf("%s%s\n", base_ansi, sprite[y]);
  }
  return 0;
}


const box DEFAULT = (box){
    .top = "═",
    .top_left_corner = "╔",
    .top_right_corner = "╗",
    .bottom = "═",
    .bottom_left_corner = "╚",
    .bottom_right_corner = "╝",
    .left = "║",
    .right = "║",
    .top_left_title_border = "╣",
    .top_right_title_border = "╠",
    .bottom_left_title_border = "╣",
    .bottom_right_title_border = "╠",
    .title_padding = 1,
    .top_left_title = "box",
    .top_right_title = "",
    .bottom_left_title = "",
    .bottom_right_title = "",
    .fill_char = " ",
    .top_left_title_ansi = "",
    .top_right_title_ansi = "",
    .bottom_left_title_ansi = "",
    .bottom_right_title_ansi = "",
};

typedef struct {
  int x;
  int y;
  int width;
  int height;
  char *color;
} solid_opt;

int solid(const solid_opt loc) {
  char base_ansi[64];

  hex2ansi_buf("#000000", loc.color, base_ansi, sizeof base_ansi);
  box draw = DEFAULT;
  draw.x = loc.x;
  draw.y = loc.y;
  draw.width = loc.width;
  draw.height = loc.height;
  draw.ansi = base_ansi;

  draw_box(draw);
  fflush(stdout);
  return 0;
}

typedef struct {
  int x;
  int y;
  int height;
  int width;
  char *foreground;
  char *background;

} box_small;

int main() {
  const char *cat[] = {
      " ╱|",
      "(˚ˎ 。7",
      " |、˜〵",
      "じしˍ,)ノ",
  };

  termsize local = get_term_size();

  // height halves
  int h1 = local.ROW - (local.ROW / 2);
  int h2 = local.ROW - h1;

  // width halves
  int w1 = local.COL - (local.COL / 2);
  int w2 = local.COL - w1;

  // print red half
  solid((solid_opt){
      .x = 0,
      .y = 0,
      .width = w1,
      .height = local.ROW,
      .color = "#ff0000",
  });

  // print green quater
  solid((solid_opt){
      .x = w1,
      .y = 0,
      .width = w1,
      .height = h1,
      .color = "#00ff00",
  });

  // print blue quater
  solid((solid_opt){
      .x = w1, .y = h1, .width = w2, .height = h2, .color = "#0000ff"});

  // draw cat sprite
  draw_sprite(
      (sprite_opt){
          .x = w1 + 1,
          .y = h1 + 1,
          .height = 4,
          .width = 15,
          .background = "#0000ff",
          .foreground = "#000000",
      },
      cat);

  return 0;
}
