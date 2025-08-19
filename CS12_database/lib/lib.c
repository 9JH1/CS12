#include <stdio.h> 
#include <time.h>
#include <stdlib.h> 
#include <string.h>
#include "lib.h"


int struct_write(void *data, int struct_size, char *filename){
	FILE *out = fopen(filename,"wb");
	if(out == NULL){
		printf("Error occured trying to open \"%s\"\n",filename);
		return 1;
	}

	int written = fwrite(data,struct_size,1,out);
	fclose(out);

	if(written != 1){
		printf("Error writing to file \"%s\"\n",filename);
		return 1;
	}
	
	return 0;
}

int struct_read(void *data, int struct_size, char *filename){
	FILE *in = fopen(filename, "rb");
	if(in == NULL){
		printf("Error occured trying to open \"%s\"\n",filename);
		return 1;
	}

	int read = fread(data,struct_size,1,in);
	fclose(in);

	if(read != 1){
		printf("Error reading from file \"%s\"\n",filename);
		return 1;
	}

	return 0;
}

void input(char *buffer, int size, char* prompt) {
	printf("%s", prompt);
  if (fgets(buffer, size, stdin) != NULL) {
  	// Remove newline character if present
  	int len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
    	buffer[len - 1] = '\0';
      }
  }
}


char random_char(void) {
    // Seed the random number generator (call once in program)
    static int seeded = 0;
    if (!seeded) {
        srand(time(NULL));
        seeded = 1;
    }
    
    // Generate random printable ASCII character (from 33 to 126)
    return (char)(rand() % (126 - 33 + 1) + 33);
}
