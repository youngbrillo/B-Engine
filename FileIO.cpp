#define _CRT_SECURE_NO_WARNINGS
#include "FileIO.h"
#include "FileData.h"

bool FileIO::LoadFile(char*& data, int& size, const char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (file == nullptr)
	{
		printf("FILEIO::ReadFile::Error\t cannot open file '%s'", filename);
		return false;
	}

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (size == 0)
	{
		printf("FILEIO::ReadFile::Error\t file '%s': has no data", filename);
		return false;
	}

	data = (char*)malloc(size + 1);
	fread(data, size, 1, file);
	fclose(file);
	data[size] = 0;

	return true;
}

bool FileIO::SaveFile(const char* fileName, FileData* data)
{
	printf("Saving File '%s':\n", fileName);
	FILE* file = fopen(fileName, "w");
	data->onSave(file);
	fclose(file);
	return false;
}



