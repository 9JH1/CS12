/* @file ui.c
 * @breif mini ui library
 * @details this file contains all ui related functions used by the database
 **/

#include "lib/hlib/lib/ansi.h"
#include "lib/hlib/lib/draw.h"
#include "lib/lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define COL_SIZE_2 65
#define COL_SIZE 55
void ui_quit(int code) {
  show_cursor();
  exit(code);
}

void ui_print(const char *in);

/**
 * @breif wrapper function for ui_menu
 * @param array array of strings used in menu
 * @param prompt prompt string shown at top of menu
 * @return ui_menu
 * @details wraps around ui_menu using a macro which removes the need for the
 * 					user to enter the array size.
 **/
#define as(a) sizeof(a) / sizeof(a[0])
#define ui_m(a1, a2, p) ui_menu(a1, as(a1), a2, p);

int run_count;

/**
 * @brief draws a interactive menu
 * @param *array[] array of strings used in menu
 * @param size amount of items in *array[]
 * @param *prompt prompt strings shown at top of menu
 * @return returns the selected index int of the menu
 * @details uses an array of strings as items and draws a menu, use of UPARROW
 * 					and DOWNARROW can navigate the menu
 *pressing enter returns the selected integer index of the array
 **/
int ui_menu(const char *array1[], const int size, const char *array2[],
            const char *prompt) {
  hide_cursor();
  run_count = 0;

  // calculate longest
  int x, y, selected;
  y = size + 1;
  x = 9 + COL_SIZE + COL_SIZE_2;
  selected = 0;

  solid_opt box_s = {
      .width = x + 2,
      .height = y + 2,
      .x = 1,
      .y = 1,
  };

  border_opt bor_s = {
      .bottom = ".",
      .top = ".",
      .left = "|",
      .right = "|",
      .bottom_left_corner = "`",
      .bottom_right_corner = "'",
      .top_left_corner = ",",
      .top_right_corner = ".",
  };

  border(box_s, &bor_s);

  while (1) {
    clear();
    date rn = date_now();

    printf("%s\033[0m\n", bor_s.string);
    gotoxy(0, 0);

    printf("%s (%d total lines) (current datetime: %d/%d/%d %d:%d:%d)\n",
           prompt, size, rn.day, rn.month, rn.year, rn.hour, rn.minute,
           rn.second);

    // draw the menu
    for (int i = 0; i < size + 1; i++) {
      gotoxy(1, i + 1);
      if (i == selected)
        printf("\033[30;42;1m");

      if (i != size) {
        if ((COL_SIZE - strlen(array1[i])) <= 0) {
          printf("Error line too long: '%s'\n", array1[i]);
          continue;
        }

        if ((COL_SIZE - strlen(array2[i])) <= 0) {
          printf("Error line too long: '%s'\n", array2[i]);
          continue;
        }
        printf("%2d. | %s", i + 1, array1[i]);

        for (int j = 0; j < COL_SIZE - strlen(array1[i]); ++j)
          printf(" ");
        printf(" | ");
        if (run_count == 0)
          ui_print(array2[i]);
        else
          printf("%s", array2[i]);

        for (int j = 0; j < COL_SIZE_2 - strlen(array2[i]); ++j)
          printf(" ");
        printf("\033[0m\n");
      } else {
        printf("Back");
        for(int j = 0; j < x - 4;j++)
                printf(" ");
        printf("\033[0m\n");
      }
    }

    // handle the input
    int ch = achar();
    switch (ch) {
    case 'B':
      if (selected == size)
        selected = 0;
      else
        selected++;
      break;

    case 'A':
      if (selected == 0)
        selected = size;
      else
        selected--;
      break;

    case 10:
      show_cursor();
      free(bor_s.string);
      printf("\n\n");
      if (selected == size)
        return -2;
      return selected;
    }

    run_count++;
  }

  free(bor_s.string);
  show_cursor();
  printf("\n\n");
  return -1;
}

void ui_print(const char *in) {
  int line_size = 0;
  int enter_flag = 0;
  size_t len = strlen(in); // Cache the string length

  for (size_t i = 0; i < len; i++) {
    usleep(500);
    fflush(stdout);
    const char cur = in[i];

    // Handle existing newlines in the input
    if (cur == '\n') {
      printf("\n");
      line_size = 0;
      enter_flag = 0;
      continue;
    }

    // Check if line length exceeds the threshold
    if (line_size >= COL_SIZE + COL_SIZE_2) {
      enter_flag = 1;
    }

    // If enter_flag is set and current character is a space, insert newline
    if (enter_flag && cur == ' ') {
      printf("\n");
      line_size = 0;
      enter_flag = 0;
    } else {
      printf("%c", cur);
      line_size++; // Increment line_size for each character printed
    }
  }
}
