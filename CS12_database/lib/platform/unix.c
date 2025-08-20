#include "unix.h"
#include <dirent.h> 

int dir_exist(const char *path) {
	DIR *dir = opendir(path);
  if (dir) {
  	closedir(dir);
    return 1; 
  }
	return 0;
}
