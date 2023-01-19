#include "jsonParser.h"

void LoadJsonFile(const char* fileName)
{

	char* data = nullptr;
	int size = 0;
	bool found = ReadFile(data, size, fileName);
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

	printf("fieldCount: %d\n", fieldCount);
	for (int i = 0; i < fieldCount; i++)
	{
		sajson::string fieldName = root.get_object_key(i);
		sajson::value fieldValue = root.get_object_value(i);
		outputJsonContents(0, i, fieldName.as_string(), fieldValue, fieldValue.get_type());
	}

}


void outputJsonContents(int depth, const int& index, const std::string& fieldName, const sajson::value& fieldvalue, const sajson::type& type)
{
	for (int k = 0; k < depth; k++) { printf("\t"); }
	printf("%d) fieldName: '%s'", index, fieldName.c_str());//, fieldValue.as_cstring());
	int fieldCount;
	std::string placeholderstring;
	//sajson::string name;// = fieldvalue.get_object_key(i);
	//sajson::value value;// = fieldvalue.get_object_value(i);
	switch (type)
	{
	case sajson::type::TYPE_OBJECT:
		fieldCount = int(fieldvalue.get_length());
		printf(" is <object> of length:\t%d\n", fieldCount);

		for (int k = 0; k < depth; k++) { printf("\t"); }
		for (int i = 0; i < fieldCount; i++)
		{
			sajson::string name = fieldvalue.get_object_key(i);
			sajson::value value = fieldvalue.get_object_value(i);
			outputJsonContents(depth + 1, i, name.as_string(), value, value.get_type());
		}
		break;
	case sajson::type::TYPE_ARRAY:
		fieldCount = int(fieldvalue.get_length());
		printf(" is <array> of length:\t%d\n", fieldCount);
		for (int k = 0; k < depth; k++) { printf("\t"); }

		for (int i = 0; i < fieldCount; i++)
		{
			sajson::value value = fieldvalue.get_array_element(i);
			outputJsonContents(depth + 1, i, "Array value", value, value.get_type());
		}
		break;
	case sajson::type::TYPE_DOUBLE:
		printf(" is <double>\t%.2f\n", fieldvalue.get_double_value());
		break;
	case sajson::type::TYPE_INTEGER:
		printf(" is <integer>\t%d\n", fieldvalue.get_integer_value());
		break;
	case sajson::type::TYPE_STRING:
		printf(" is <string> \t%s\n", fieldvalue.as_string().c_str());
		//printf(" fieldValue: %s\n", value.as_cstring());
		break;
	case sajson::type::TYPE_NULL:
		printf(" is <null>\n");
		break;

	case sajson::type::TYPE_FALSE:
		printf(" is <false>\n");
		break;

	case sajson::type::TYPE_TRUE:
		printf(" is <true>\n");
		break;
	default:
		break;
	}
}

bool ReadFile(char*& data, int& size, const char* filename)
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