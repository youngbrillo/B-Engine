#pragma once
#define SCORE_FILE_MAX_SCORE 14
#include <stdio.h>
#include <vector>
namespace FileIO
{
	class FileData
	{
	public:
		FileData(const char* FileName);

		virtual ~FileData() { data = nullptr; };
		virtual void onSave(FILE* file) = 0;
		virtual void onLoad() = 0; 
	protected:
		char* data;
		int dataSize;
	};

	class ScoreFile : public FileData
	{
	public:
		ScoreFile(const char* FileName);
		~ScoreFile() {}
		void onSave(FILE* file);
		void onLoad() override;

	public:
		int highestScore, currentScore;
		std::vector<int> scores;
	};
}

