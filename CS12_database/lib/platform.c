#include "lib.h"

#ifdef __unix__
#include <dirent.h>
#include <termios.h>
#include <unistd.h>

int file_exist(const char *path) {
  if (access(path, F_OK) == 0)
    return 1;
  return 0;
}

int dir_exist(const char *path) {
  DIR *dir = opendir(path);
  if (dir) {
    closedir(dir);
    return 1;
  }
  return 0;
}

int achar() {
  static struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  int ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return ch;
}

#elif _WIN32
#include <io.h>
#include <windows.h>

int file_exist(const char *path) {
  if (_access(path, 0) == 0)
    return 1;
  return 0;
}

int dir_exist(const char *path) {
  DWORD attrib = GetFileAttributesA(path);
  if (attrib != INVALID_FILE_ATTRIBUTES &&
      (attrib & FILE_ATTRIBUTE_DIRECTORY)) {
    return 1;
  }
	
  return 0;
}

int achar() {
  HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
  DWORD mode, bytesRead;
  INPUT_RECORD ir;
  KEY_EVENT_RECORD ker;
  GetConsoleMode(hStdin, &mode);
  SetConsoleMode(hStdin, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
  ReadConsoleInput(hStdin, &ir, 1, &bytesRead);
  if (ir.EventType == KEY_EVENT && ir.Event.KeyEvent.bKeyDown) {
    ker = ir.Event.KeyEvent;
    char ch = ker.uChar.AsciiChar;
    if (ch == '\r') {
      ch = '\n';
    } else if (ker.wVirtualKeyCode >= VK_UP && ker.wVirtualKeyCode <= VK_DOWN) {
      switch (ker.wVirtualKeyCode) {
      case VK_UP:
        ch = 'A';
        break;
      case VK_DOWN:
        ch = 'B';
        break;
      case VK_RIGHT:
        ch = 'C';
        break;
      case VK_LEFT:
        ch = 'D';
        break;
      }
    }
    SetConsoleMode(hStdin, mode);
    return (unsigned char)ch;
  }

  SetConsoleMode(hStdin, mode);
  return -1;
}

#endif
