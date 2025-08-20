#include "lib.h"

#ifdef __unix__
	#include <dirent.h>
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

#elif _WIN32
	#include <io.h>
	#include <windows.h>

	int file_exist(const char *path)
		if (_access(fname, 0) == 0) return 1;
		return 0;
	}

	int dir_exist(const char *path) {
    DWORD attrib = GetFileAttributesA(path);
    if (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY)) {
        return 1;
  	}
  	  return 0; 
	}
#endif
