#pragma once
#include <string>
#include <stdlib.h>
#include <stdio.h>

namespace FileIO
{
	class FileData; //forward reference
	bool LoadFile(char*& data, int& size, const char* filename);
	bool SaveFile(const char* fileName, FileData* data);

}