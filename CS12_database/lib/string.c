#include <stdio.h>

// String compare
inline int scmp(const char *s1, const char *s2) {
  if (s1 == NULL && s2 == NULL)
    return 0;
  if (s1 == NULL)
    return -1;
  if (s2 == NULL)
    return 1;
  while (*s1 && (*s1 == *s2)) {
    s1++;
    s2++;
  }
  return (unsigned char)*s1 - (unsigned char)*s2;
}

// string length
inline int _slen(const char *a) {
  int out = 0;
  for (int i = 0; a[i]; i++) {
    out++;
  }
  return out;
}

// string copy
inline void _scp(char *s, char *t) {
  while (*s != '\0') {
    *(t) = *(s);
    s++;
    t++;
  }
  *t = '\0';
}

// foreach (array)
#define foreach(item, array)                                                   \
  for (int keep = 1, count = 0, size = sizeof(array) / sizeof *(array);        \
       keep && count != size; keep = !keep, count++)                           \
    for (item = (array) + count; keep; keep = !keep)

// for each item
#define for_each_item(item, list)                                              \
  for (T *item = list->head; item != NULL; item = item->next)
