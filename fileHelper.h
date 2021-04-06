#pragma once

#define BINARY_FILE_BY_BITS //if define using bits,if not using the regular binary
#define BINARY_BUFFER_SIZE_COMPANY 2	//num of byte needed in  buffer's array
#define BINARY_BUFFER_SIZE_FLIGHT 4
typedef unsigned char BYTE; // we want use byte


int	 writeStringToFile(char* str, FILE* fp, const char* msg);
int	 writeCharsToFile(char* arr, int size, FILE* fp, const char* msg);
int	 writeIntToFile(int val, FILE* fp, const char* msg);

char* readStringFromFile(FILE* fp, const char* msg);
int	 readCharsFromFile(char* arr, int size, FILE* fp, const char* msg);
int	  readIntFromFile(int* val, FILE* fp, const char* msg);
