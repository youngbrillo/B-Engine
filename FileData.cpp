#define _CRT_SECURE_NO_WARNINGS

#include "FileData.h"
#include "sajson.h"
#include "FileIO.h"


FileIO::FileData::FileData(const char* FileName)
	: data(nullptr), dataSize(0)
{
}

FileIO::ScoreFile::ScoreFile(const char* FileName)
	: FileData(FileName)
	, highestScore(0)
	, currentScore(0)
	, scores {0}
{
	if (LoadFile(data, dataSize, FileName))
	{
		printf("Got file data from %s\n", FileName);
		onLoad();
	}
}

#include<vector>
void FileIO::ScoreFile::onSave(FILE* file)
{
	//add new score
	scores.push_back(currentScore);
	//sort scores greatest - least
	std::sort(scores.begin(), scores.end(), std::greater<int>());
	//get top N scores , N = 14 in this case
	scores.resize(SCORE_FILE_MAX_SCORE);

	printf("\tSaving Score File\n");
	fprintf(file, "{\n");
	fprintf(file, "  \"highScore\": %d,\n", this->highestScore);
	fprintf(file, "  \"scores\":[");
	char iter = 0x20; // ''
	for (int i = 0; i < scores.size(); i++)
	{
		fprintf(file, "%c %d", iter, scores[i]);
		iter = ',';
	}
	fprintf(file, "]\n}\n");

}

typedef  sajson::value myJsonValue;

void FileIO::ScoreFile::onLoad()
{
	printf("\tLoading Score file\n");
	//assuming that the scores here are sorted from lowest to highest
	const sajson::document& document = sajson::parse(sajson::dynamic_allocation(), sajson::mutable_string_view(dataSize, data));
	if (document.is_valid() == false)
	{
		int line = document.get_error_line();
		int column = document.get_error_column();
		const char* message = document.get_error_message_as_cstring();
		printf("Client ScoreFile is not valid\n\tLine %d, column %d:\t%s\n", line, column, message);
		return;

	}
	sajson::value root = document.get_root();
	int fieldCount = int(root.get_length());

	for (int i = 0; i < fieldCount; i++)
	{
		sajson::string fieldName = root.get_object_key(i);
		sajson::value fieldValue = root.get_object_value(i);
		if (fieldName.as_string() == "highScore" && fieldValue.get_type() == sajson::type::TYPE_INTEGER)
		{
			highestScore = fieldValue.get_integer_value();
		}

		if (fieldName.as_string() == "scores" && fieldValue.get_type() == sajson::type::TYPE_ARRAY)
		{
			int arrayLength = int(fieldValue.get_length());
			for (int ii = 0; ii < arrayLength; ii++)
			{
				//sajson::value value = fieldValue.get_array_element(ii);
				myJsonValue value = fieldValue.get_array_element(ii);
				//printf("%d: Score %d\n", ii,  value.get_integer_value() );
				if (value.get_type() == sajson::type::TYPE_INTEGER)
				{
					//this->scores[ii] = value.get_integer_value();
					scores.push_back(value.get_integer_value());
				}
				else {
					//scores.push_back(0);
				}
			}
		}
	}
}
