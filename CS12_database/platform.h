// this code just defines variables based off of what playform 
// the computer is on, it should be pretty straight forward as 
// to what this code does.



#ifndef PLATFORM_H
	// platform selection: 
	#ifdef __unix__
		// if on unix: 
		#define DELETE_COMMAND "rm -rf"
		#define MKDIR_COMMAND "mkdir"
		#include "lib/platform/unix.h"
	#elif _WIN32
		// if on windows
		#define DELETE_DOMMAND "del /s /q"
		#define MKDIR_COMMAND "mkdir" // or md
		#include "lib/platform/win32.h"
	#else 
		printf("Platform Not Supported! please use either Windows (_WIN32) or Linux (_unix_)\n");
		exit(1);
	#endif
#endif // PLATFORM_H
