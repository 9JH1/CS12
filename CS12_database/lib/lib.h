#ifndef DATABASE_LIB 
#define DATABASE_LIB

char random_char(void);
void input(char *buffer, int size, char* prompt);
int struct_write(void *data, int struct_size, char *filename);
int struct_read(void *data, int struct_size, char *filename);
#endif // !DATABASE_LIB 
