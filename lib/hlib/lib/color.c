#include "color.h"
#include <stdio.h>
#include <stdlib.h>

int hexpair_to_int(const char *hex) {
  char part[3] = {hex[0], hex[1], '\0'};
  return (int)strtol(part, NULL, 16);
}

void hex2ansi_buf(const char *foreground_hex, const char *background_hex,
                  char *out, int out_size) {
  if (foreground_hex[0] == '#')
    foreground_hex++;
  if (background_hex[0] == '#')
    background_hex++;

  int fr = hexpair_to_int(foreground_hex);
  int fg = hexpair_to_int(foreground_hex + 2);
  int fb = hexpair_to_int(foreground_hex + 4);

  int br = hexpair_to_int(background_hex);
  int bg = hexpair_to_int(background_hex + 2);
  int bb = hexpair_to_int(background_hex + 4);

  if (!foreground_hex) {
    snprintf(out, out_size, "\033[38;2;%d;%d;%dm", br, bg, bb);

  } else if (!background_hex) {
    snprintf(out, out_size, "\033[38;2;%d;%d;%dm", fr, fg, fb);

  } else {
    snprintf(out, out_size, "\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm", fr, fg,
             fb, br, bg, bb);
  }
}

char *color(const char *foreground, const char *background) {
  static char out[64];
  hex2ansi_buf(foreground, background, out, 64);
  return out;
}
