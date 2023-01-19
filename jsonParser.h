#define _CRT_SECURE_NO_WARNINGS
#include "sajson.h"

/*
*	recursive function that prints the contents of a JSON file/String
*/
void outputJsonContents(int depth, const int& index, const std::string& filedname, const sajson::value& fieldvalue, const sajson::type& type);
void LoadJsonFile(const char* fileName);
bool ReadFile(char*& data, int& size, const char* filename);
