#ifndef COLOR_H
#define COLOR_H
#define COLOR_ANSI_BYTE_SIZE 64;

int hexpair_to_int(const char *hex);
void hex2ansi_buf(const char *foreground_hex, const char *background_hex,
                  char *out, int out_size);
char *color(const char *foreground, const char *background);
#endif
