#ifndef FILE_FUNCTIONS_H
#define FILE_FUNCTIONS_H


#include <fstream>
#include <iostream>
using namespace std;


bool file_exists(const char filename[]);
void open_fileRW(fstream& f, const char filename[]) throw(char*);
void open_fileW(fstream& f, const char filename[]);

//Example
void fun_with_binary_file();

#endif // FILE_FUNCTIONS_H
