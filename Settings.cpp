#define _CRT_SECURE_NO_WARNINGS

#include "Settings.h"
#include <stdio.h>
#include <stdlib.h>
#include "sajson.h"

static const char* fileName = "settings.ini";

// Load a file. You must free the character array.
static bool sReadFile(char*& data, int& size, const char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (file == nullptr)
	{
		return false;
	}

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (size == 0)
	{
		return false;
	}

	data = (char*)malloc(size + 1);
	fread(data, size, 1, file);
	fclose(file);
	data[size] = 0;

	return true;
}


void Settings::Save()
{
	FILE* file = fopen(fileName, "w");
	fprintf(file, "{\n");
	fprintf(file, "  \"testIndex\": %d,\n", m_index);
	fprintf(file, "  \"windowWidth\": %d,\n", m_windowWidth);
	fprintf(file, "  \"windowHeight\": %d\n", m_windowHeight);
	/*fprintf(file, "  \"hertz\": %.9g,\n", m_hertz);
	fprintf(file, "  \"velocityIterations\": %d,\n", m_velocityIterations);
	fprintf(file, "  \"positionIterations\": %d,\n", m_positionIterations);
	fprintf(file, "  \"drawShapes\": %s,\n", m_drawShapes ? "true" : "false");
	fprintf(file, "  \"drawJoints\": %s,\n", m_drawJoints ? "true" : "false");
	fprintf(file, "  \"drawAABBs\": %s,\n", m_drawAABBs ? "true" : "false");
	fprintf(file, "  \"drawContactPoints\": %s,\n", m_drawContactPoints ? "true" : "false");
	fprintf(file, "  \"drawContactNormals\": %s,\n", m_drawContactNormals ? "true" : "false");
	fprintf(file, "  \"drawContactImpulse\": %s,\n", m_drawContactImpulse ? "true" : "false");
	fprintf(file, "  \"drawFrictionImpulse\": %s,\n", m_drawFrictionImpulse ? "true" : "false");
	fprintf(file, "  \"drawCOMs\": %s,\n", m_drawCOMs ? "true" : "false");
	fprintf(file, "  \"drawStats\": %s,\n", m_drawStats ? "true" : "false");
	fprintf(file, "  \"drawProfile\": %s,\n", m_drawProfile ? "true" : "false");
	fprintf(file, "  \"enableWarmStarting\": %s,\n", m_enableWarmStarting ? "true" : "false");
	fprintf(file, "  \"enableContinuous\": %s,\n", m_enableContinuous ? "true" : "false");
	fprintf(file, "  \"enableSubStepping\": %s,\n", m_enableSubStepping ? "true" : "false");
	fprintf(file, "  \"enableSleep\": %s\n", m_enableSleep ? "true" : "false");*/
	fprintf(file, "}\n");
	fclose(file);

}

void Settings::Load()
{
	char* data = nullptr;
	int size = 0;
	bool found = sReadFile(data, size, fileName);
	if (found == false)
	{
		printf("Could not load settings file...%s\n", fileName);
		return;
	}

	const sajson::document& document = sajson::parse(sajson::dynamic_allocation(), sajson::mutable_string_view(size, data));
	if (document.is_valid() == false)
	{
		int line = document.get_error_line();
		int column = document.get_error_column();
		const char* message = document.get_error_message_as_cstring();
		printf("Settings File (%s) is not valid\n\tLine %d, column %d:\t%s\n", fileName, line, column, message);
		return;
	}

	sajson::value root = document.get_root();
	int fieldCount = int(root.get_length());
	for (int i = 0; i < fieldCount; ++i)
	{
		sajson::string fieldName = root.get_object_key(i);
		sajson::value fieldValue = root.get_object_value(i);

		if (strncmp(fieldName.data(), "testIndex", fieldName.length()) == 0)
		{
			if (fieldValue.get_type() == sajson::TYPE_INTEGER)
			{
				m_index = fieldValue.get_integer_value();
			}
			continue;
		}

		if (strncmp(fieldName.data(), "windowWidth", fieldName.length()) == 0)
		{
			if (fieldValue.get_type() == sajson::TYPE_INTEGER)
			{
				m_windowWidth = fieldValue.get_integer_value();
			}
			continue;
		}

		if (strncmp(fieldName.data(), "windowHeight", fieldName.length()) == 0)
		{
			if (fieldValue.get_type() == sajson::TYPE_INTEGER)
			{
				m_windowHeight = fieldValue.get_integer_value();
			}
			continue;
		}

	//	if (strncmp(fieldName.data(), "hertz", fieldName.length()) == 0)
	//	{
	//		if (fieldValue.get_type() == sajson::TYPE_DOUBLE || fieldValue.get_type() == sajson::TYPE_INTEGER)
	//		{
	//			m_hertz = float(fieldValue.get_number_value());
	//		}
	//		continue;
	//	}

	//	if (strncmp(fieldName.data(), "velocityIterations", fieldName.length()) == 0)
	//	{
	//		if (fieldValue.get_type() == sajson::TYPE_INTEGER)
	//		{
	//			m_velocityIterations = fieldValue.get_integer_value();
	//		}
	//		continue;
	//	}

	//	if (strncmp(fieldName.data(), "positionIterations", fieldName.length()) == 0)
	//	{
	//		if (fieldValue.get_type() == sajson::TYPE_INTEGER)
	//		{
	//			m_positionIterations = fieldValue.get_integer_value();
	//		}
	//		continue;
	//	}

	//	if (strncmp(fieldName.data(), "drawShapes", fieldName.length()) == 0)
	//	{
	//		if (fieldValue.get_type() == sajson::TYPE_FALSE)
	//		{
	//			m_drawShapes = false;
	//		}
	//		else if (fieldValue.get_type() == sajson::TYPE_TRUE)
	//		{
	//			m_drawShapes = true;
	//		}
	//		continue;
	//	}
	}


	printf("Successfully Loaded file: %s\n", fileName);
}
