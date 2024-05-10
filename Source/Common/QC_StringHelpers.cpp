#include "QC_StringHelpers.h"

#include "QC_Math.h"

char* StringAppend(const char* a, const char* b)
{
	const size_t size = strlen(a) + strlen(b);

	char* newString = new char[size]; // #TODO Delete memory. Maybe use a smart pointer.
	strcpy_s(newString, strlen(a) + 1, a);
	strcat_s(newString, size + 1, b);

	return newString;
}

// #TODO String append using variable number of arguments
// #TODO Review usage and memory allocations
char* StringAppend(const char* a, const char* b, const char* c)
{
	// #TODO Refactor string append methods to properly manage string allocations
	const size_t size = strlen(a) + strlen(b) + strlen(c);

	char* newString = new char[size]; // #TODO Delete memory. Maybe use a smart pointer.
	strcpy_s((char*)newString, strlen(a) + 1, a);
	strcat_s((char*)newString, size + 1, b);
	strcat_s((char*)newString, size + 1, c);

	return newString;
}

char* DeepCopyString(const char* string)
{
	return _strdup(string);
}

char* CombineStrings(const char* a, const char* b) // #TODO Review const return value
{
	// #TODO Review method
	// Cannot use an initialized string
	const size_t size = strlen(a) + strlen(b) + 1;
	char* buffer = new char[size];

	// Overwrite "string" to buffer
	strcpy_s(buffer, size, a); // (char* buffer, bufferSize, const char* "string")
	// Append "string" to buffer
	strcat_s(buffer, size, b); // (buffer, totalSize, "string")

	return buffer;
}

std::string Concatenate(const char* a, const char* b)
{
	std::string value = a;
	value.append(b);

	return value;
}

bool StringCompare(const char* a, const char* b) // #TODO Rename for boolean return type
{
	return strcmp(a, b) == 0;
}

bool StrCmp(std::string a, std::string b) // #TODO Rename for boolean return type
{
	return strcmp(a.c_str(), b.c_str()) == 0;
}

bool StringCompare(std::string a, std::string b) // #TODO Rename for boolean return type
{
	return strcmp(a.c_str(), b.c_str()) == 0;
}

bool StringHasText(std::string content, std::string searchText)
{
	return content.find(searchText) != content.npos;
}

int StringToInt(const char* string)
{
	return std::stoi(string, nullptr, 10); // #TODO Test
}

char* ReverseString(char* value)
{
	// #TODO Debug
	const size_t size = strlen(value);
	std::string t_temp = std::string(size, ' ');

	for (size_t i = 0; i < size; i++)
	{
		t_temp.push_back(value[size - i - 1]);
	}

	memcpy((char*)value, t_temp.c_str(), size + 1);
	return value; // #TODO Review removing return value
}

char NumberIntToChar(const int& integer)
{
	return integer + 48;
}

int NumberCharToInt(const char& character)
{
	if (character >= 48 && character <= 57)
	{
		return character - 48;
	}
	return character;
}

bool IsNumber(const char& number)
{
	if (number >= 48 && number <= 57)
	{
		return true;
	}
	return false;
}

bool IsLetter(const char& letter)
{
	if (letter >= 65 && letter <= 90)
	{
		return true;
	}
	else if (letter >= 97 && letter <= 122)
	{
		return true;
	}
	return false;
}

// #TODO Consider implementing a test case
// #TODO Load libraries dynamically. Need functions to load .dlls
// #TODO Try to reduce or avoid order dependency in system creation

// const char* newName = new char[5] { 'N','a','m', 'e', '\0' };
// unsigned  long long counter = UINT64_MAX - 1;
// const unsigned long long max = UINT64_MAX - 1;
// // const char* newName = new char[12] { 'N', '2', '1', '4', '7', '4', '8', '3', '6', '4', '7', '\0' };
// const char* newName = new char[22] { 'N', '1', '8', '4', '4', '6', '7', '4', '4', '0', '7', '3', '7', '0', '9',
// 	'5', '5', '1', '6', '1', '4', '\0' };
//
// while (true)
// {
// 	const char* oldName = newName;
// 	newName = NumberAppendOrIncrement(newName);
// 	delete[] oldName;
//
// 	Log::Console(newName);
// 	Log::Console("\n");
//
// 	std::string value("N");
// 	value.append(std::to_string(counter + 1));
// 	if (value != newName)
// 	{
// 		int bp = 0;
// 	}
//
// 	if (counter % 100 == 0)
// 	{
// 		int bp = 0;
// 	}
// 	++counter;
// }
// delete[] newName;

char* NumberAppendOrIncrement(const char* const charArray)
{
	if (!charArray)
		return nullptr;

	const unsigned int charArrayLength = strlen(charArray);
	bool carryValue = false;
	unsigned long long valueToAppend = 0;
	unsigned short numFoundDigits = 0;

	for (int r = charArrayLength - 1; r >= 0; r--)
	{
		const int iterationNumber = charArrayLength - r;
		char character = charArray[r];
		if (IsNumber(character))
		{
			++numFoundDigits;
			unsigned char result = NumberCharToInt(character);
			if (iterationNumber == 1 || carryValue)
			{
				result += 1;
				carryValue = false;
			}

			if (result > 9)
			{
				carryValue = true;
			}

			result = result % 10;
			// const int exponent = iterationNumber == 1 ? 0 : iterationNumber;
			valueToAppend += result * QwerkE::Math::Pow(10, iterationNumber - 1);
		}
		else
		{
			break;
		}
	}

	if (carryValue)
	{
		valueToAppend += (char)carryValue * QwerkE::Math::Pow( 10, numFoundDigits - 1) * 10;
	}

	if (charArrayLength >= sizeof(long long))
	{
		// #TODO Support larger strings. Could use charArrayLength to create
		// a new array of bytes and access digits in blocks or bits.
		// Append 0 for now.
		int bp = 1;
	}

	if (valueToAppend == 0)
	{
		carryValue = true;
	}

	const unsigned int returnCharArrayLength = charArrayLength + (int)carryValue;
	char* returnCharArray = new char[returnCharArrayLength + 1];
	memcpy(returnCharArray, charArray, charArrayLength);
	returnCharArray[returnCharArrayLength] = '\0';

	// #TODO Handle digits == 0
	unsigned int digits = (unsigned int)QwerkE::Math::Digits(valueToAppend);

	unsigned long long remainingValue = valueToAppend;
	for (size_t i = returnCharArrayLength - digits; i < returnCharArrayLength; i++)
	{
		const int digitNumber = returnCharArrayLength - i;

		unsigned char newCharValue = (unsigned char)(remainingValue / QwerkE::Math::Pow(10, digitNumber - 1));
		while (newCharValue > 9)
		{
			newCharValue -= 10;
		}
		remainingValue -= newCharValue * QwerkE::Math::Pow(10, digitNumber - 1);
		returnCharArray[i] = NumberIntToChar(newCharValue);
	}

	return returnCharArray;
}

#include <windows.h>    // For getting user name
#include <Lmcons.h>     // For getting user name
// Copied from : https://stackoverflow.com/questions/215963/how-do-you-properly-use-widechartomultibyte
// #TODO Move to a helpers file
std::string wstrtostr(const std::wstring& wstr)
{
	// Convert a Unicode string to an ASCII string
	std::string strTo;
	char* szTo = new char[wstr.length() + 1];
	szTo[wstr.size()] = '\0';
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
	strTo = szTo;
	delete[] szTo;
	return strTo;
}
