#include "input.h"

int main() {
  while (1) {
    InputEvent local;
    input(&local);
    log_InputEvent(local);
  }
}
