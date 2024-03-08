#include "QF_FileUtilities.h"

#include <comdef.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <fileapi.h>
#include <winnt.h>

#include "QC_StringHelpers.h"

#include "QF_Log.h"
#include "QF_Platform.h"

// #TODO Review and improve. Could create multiple helpers here to :
// - Combining and sub stringing strings
// - Increment the number at the end of a file name
// - Could create string helpers in a namespace like String:: for organization
char* UniqueFileNameNumberAppend(const char* fileDirectory, const char* fileName, const char* extension)
{
	if (!fileDirectory || !fileName || !extension)
		return nullptr;

	// #TODO Use NumberAppendOrIncrement()
	std::string uniqueFileName = fileName;
	int counter = 0;

	while (FileExists((fileDirectory + uniqueFileName + "." + extension).c_str()))
	{
		uniqueFileName.append(std::to_string(counter));
		++counter;
	}
	return strdup((fileDirectory + uniqueFileName + "." + extension).c_str()); // #TODO Smart pointer de-allocate
}

bool FileExists(const char* filePath) // TODO:: Move to helpers.h/.cpp
{
	FILE* filehandle;
	errno_t error = fopen_s(&filehandle, filePath, "r"); // returns error if no file to read
	if (filehandle) // #TODO Read error value
	{
		fclose(filehandle);
		return true;
	}
	// OutputPrint("\nFileExists(): Could not find file. Ensure %s exists!\n\n", filePath);
	return false;
}

#ifdef _QWindows // #TODO Move to a QC_DirectoryUtilities file, to keep this file smaller, and specific
#include <windows.h>
#include <handleapi.h> // INVALID_HANDLE_VALUE
#include <vector>
// https://msdn.microsoft.com/en-us/library/windows/desktop/aa364418(v=vs.85).aspx
// https://msdn.microsoft.com/en-us/library/windows/desktop/aa365740(v=vs.85).aspx
// https://stackoverflow.com/questions/26475540/c-having-problems-with-a-simple-example-of-findfirstfile
// https://docs.microsoft.com/en-us/cpp/standard-library/filesystem-functions
std::unique_ptr<std::vector<std::string>> ReadDir(const char* directoryPath)
{
	std::unique_ptr<std::vector<std::string>> dirFileNames = std::make_unique<std::vector<std::string>>();
	dirFileNames.get()->reserve(10);

	WIN32_FIND_DATA ffd;
	HANDLE hand = INVALID_HANDLE_VALUE; // file handle
	std::string dir = directoryPath; // used for easy appending

	dir.append("*.*"); // append "search for all" instruction
	hand = FindFirstFile(_bstr_t(dir.c_str()), &ffd); // get the first file in directory

	if (INVALID_HANDLE_VALUE == hand)
		return dirFileNames;

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// std::cout << ffd.cFileName << std::endl;
		}
		else
		{
			// std::cout << ffd.cFileName << std::endl;
			dirFileNames.get()->push_back(std::string(_bstr_t(ffd.cFileName)));
		}
	} while (FindNextFile(hand, &ffd) != 0);

	DWORD dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		// DisplayErrorBox(TEXT("FindFirstFile"));
	}

	FindClose(hand);
	return dirFileNames;
}
#else
// #include <fstream>
// #include <iostream>
// #include <string>
// #include <filesystem> // C++ 17
// TODO: Look at std::filesystem
// http://www.modernescpp.com/index.php/c-17-more-details-about-the-library
//http://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html
//struct path_leaf_string
//{
//	std::string operator()(const std::filesystem::directory_entry& entry) const
//	{
//		return entry.path().leaf().string();
//	}
//};
//
//void read_directory(const std::string& name, stringvec& v)
//{
//	std::filesystem::path p(name);
//	std::filesystem::directory_iterator start(p);
//	std::filesystem::directory_iterator end;
//	std::transform(start, end, std::back_inserter(v), path_leaf_string());
//}

// MAC
// Linux
// Android
#endif

void WriteRawBytesToFile(const char* fileName, const char* dataToWrite, int numBytes)
{
	if (!fileName || !dataToWrite)
		return;

	std::ofstream oStream;
	oStream.open(fileName, std::ios_base::binary);
	if (oStream.is_open())
	{
		oStream.write((char*)dataToWrite, numBytes);
		oStream.close();
	}
}

char* ReadRawBytesFromFile(const char* const fileName)
{
	if (!fileName)
		return nullptr;

	std::ifstream iStream;
	iStream.open(fileName, std::ios_base::binary);

	if (iStream.is_open())
	{
		int size = 0;
		iStream.seekg(0, std::ios::end);
		size = (int)iStream.tellg();

		char* data = new char[size];
		iStream.read(data, size);
		iStream.close();
		return data;
	}

	return nullptr;
}

char* LoadFile(const char* fileName)
{
	FILE* filehandle;
	errno_t error = fopen_s(&filehandle, fileName, "rb");
	if (filehandle)
	{
		// find the length of the file
		fseek(filehandle, 0, SEEK_END); // go to the end
		long size = ftell(filehandle); // read the position in bytes
		rewind(filehandle); // go back to the beginning
							// before we can read, we need to allocate a buffer of the right filePathLength
		char* buffer = new char[size];
		fread(buffer, size, 1, filehandle);
		fclose(filehandle);

		return buffer;
	}
	else
	{
		return NULL;
	}
}
// TODO: Deprecate?
char* LoadCompleteFile(const char* filename, long* length = nullptr)
{
	char* filecontents = 0;

	FILE* filehandle;
	errno_t error = fopen_s(&filehandle, filename, "rb");

	if (filehandle)
	{
		fseek(filehandle, 0, SEEK_END);
		long size = ftell(filehandle);
		rewind(filehandle);

		filecontents = new char[size + 1];
		fread(filecontents, size, 1, filehandle);
		filecontents[size] = 0;

		if (length)
			*length = size;

		fclose(filehandle);
	}

	if (filecontents == 0)
	{
		LOG_ERROR("UtilityWin32: LoadCompleteFile() encountered errors loading \"{0}\"-> ", filename);
		if (error == 2)
		{
			LOG_ERROR("No such file or directory!");
		}
	}

	return filecontents;
}

// wav loading: https://blog.csdn.net/u011417605/article/details/49662535
// https://ffainelli.github.io/openal-example/
// https://stackoverflow.com/questions/13660777/c-reading-the-dataToWrite-part-of-a-wav-file/13661263
// https://stackoverflow.com/questions/38022123/openal-not-playing-sound/50429578#50429578
// http://soundfile.sapp.org/doc/WaveFormat/
unsigned char* LoadWavFileData(const char* filePath, unsigned long& bufferSize, unsigned short& channels, unsigned int& frequency, unsigned short& bitsPerSample)
{
	// TODO: Cleaner error handling
	// NOTE: I explicitly hard coded the fread() values to work cross platform

	FILE* f;
	fopen_s(&f, filePath, "rb"); // "rb" instead of "r"

	if (!f)
	{
		LOG_ERROR("LoadWaveFileData(): Error opening file: {0}", filePath);
		return nullptr;
	}

	unsigned int chunkSize = 0;
	unsigned short formatType = 0;
	unsigned int sampleRate, byteRate = 0;
	unsigned short blockAlign = 0;

	char type[4];
	char subChunk1ID[4]; // , subChunk2ID[4];

	DWORD subChunk1Size; // , subChunk2Size;

	// read first chunk
	fread(type, 4, 1, f); // ChunkID "RIFF"
	if (!strcmp(type, "RIFF"))
	{
		LOG_ERROR("LoadWaveFileData(): Not a \"RIFF\" file: {0}", filePath);
		fclose(f);
		return nullptr;
	}

	fread(&chunkSize, sizeof(DWORD), 1, f); // ChunkSize == "fmt "(4) + (8 + SubChunk1Size) + (8 + SubChunk2Size)

	fread(type, 4, 1, f);
	if (!strcmp(type, "WAVE"))
    {
        LOG_ERROR("LoadWaveFileData(): Not a \"WAVE\" file: {0}", filePath);
		fclose(f);
		return nullptr;
	}

	// read "fmt " chunk
	fread(&subChunk1ID, 4, 1, f); // "fmt "
	if (!strcmp(type, "fmt "))
    {
        LOG_ERROR("LoadWaveFileData(): No format found in file: {0}", filePath);
		fclose(f);
		return nullptr;
	}

	fread(&subChunk1Size, 4, 1, f);

	fread(&formatType, 2, 1, f); // not 1 == compression
	fread(&channels, 2, 1, f);
	fread(&sampleRate, 4, 1, f);
	fread(&byteRate, 4, 1, f);
	fread(&blockAlign, 2, 1, f);
	fread(&bitsPerSample, 2, 1, f);

	frequency = sampleRate;

	// read "dataToWrite" chunk
	fread(type, 4, 1, f);
	if (!strcmp(type, "data"))
	{
		LOG_ERROR("LoadWaveFileData(): No data in file: {0}", filePath);
		fclose(f);
		return nullptr;
	}

	fread(&bufferSize, 4, 1, f);

	unsigned char* buffer = new unsigned char[bufferSize]; //RAM: new

	int result = fread(buffer, 1, bufferSize, f);

	if (result != bufferSize || ferror(f) != 0)
    {
        LOG_ERROR("LoadWaveFileData(): Error reading data in file: {0}", filePath);
		fclose(f);
		return nullptr;
	}

	fclose(f);

	return buffer;
}

void WriteStringToFile(const char* filename, const char* string)
{
	FILE* filehandle;
	errno_t error = fopen_s(&filehandle, filename, "w+");
	if (filehandle)
	{
		fwrite(string, 1, strlen(string), filehandle); // write to file
		fclose(filehandle); // close file stream
	}
	/*
	fopen_S(&filehandle, "filePath", "mode")
	FILE* filehandle;
	char* filePath;

	Modes:
	"r"
	Opens for reading. If the file does not exist or cannot be found, the fopen_s call fails.
	"r+" = Opens for both reading and writing. (The file must exist.)

	"w"
	Opens an empty file for writing. If the file exists, its contents are destroyed.
	"w+" = Opens an empty file for both reading and writing. If the file exists, its contents are destroyed.

	"a" = Opens for writing at the end of the file (appending) without removing the EOF marker before writing new dataToWrite to the file. Creates the file if it doesn't exist.
	"a+" = Opens for reading and appending. The appending operation includes the removal of the EOF marker before
	new dataToWrite is written to the file and the EOF marker is restored after writing is complete.
	Creates the file if it doesn't exist.

	"T" = Specifies a file as temporary. If possible, it is not flushed to disk.

	"D" = Specifies a file as temporary. It is deleted when the last file pointer is closed.
	*/
}

std::unique_ptr<char> SmartFileName(const char* filePathOrName, bool includeExtension)
{
	return std::unique_ptr<char>(FileName(filePathOrName, includeExtension));
}

char* FileName(const char* filePathOrName, bool includeExtension)
{
	if (!filePathOrName)
		return nullptr;

	char* fileName = nullptr;
	int newStrSize = 0;
	const char* newStrStart = nullptr;

	if (const char* foundChar = strrchr(filePathOrName, '/'))
	{
		newStrSize = strlen(foundChar) - 1;
		newStrStart = foundChar + 1;
	}
	else if (const char* foundChar = strrchr(filePathOrName, '\\'))
	{
		newStrSize = strlen(foundChar) - 1;
		newStrStart = foundChar + 1;
	}
	else
	{
		newStrSize = strlen(filePathOrName);
		newStrStart = filePathOrName;
	}

	if (!includeExtension)
	{
		if (const char* foundChar = strrchr(filePathOrName, '.'))
		{
			newStrSize -= strlen(foundChar);
		}
	}

	if (newStrSize > 0)
	{
		fileName = new char[newStrSize + 1];
		strncpy(fileName, newStrStart, newStrSize);
		fileName[newStrSize] = '\0';
	}

	return fileName;
}

std::unique_ptr<char> SmartFileExtension(const char* filePathOrName)
{
	return std::unique_ptr<char>(FileExtension(filePathOrName));
}

char* FileExtension(const char* filePathOrName)
{
	if (!filePathOrName)
		return nullptr;

	signed short extensionStartingIndex = -1;
	const unsigned char filePathLength = (unsigned char)strlen(filePathOrName);
	for (unsigned char i = filePathLength - 1; i >= 0; --i)
	{
		if (filePathOrName[i] == '.')
		{
			extensionStartingIndex = i + 1;
			break;
		}
	}

	if (extensionStartingIndex > -1)
	{
		const char bufferSize = filePathLength - extensionStartingIndex + 1;
		char* newSubString = new char[bufferSize];
		strcpy_s(newSubString, bufferSize, filePathOrName + extensionStartingIndex);
		return newSubString;
	}

	return nullptr;
}
