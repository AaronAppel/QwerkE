#pragma once

#include <string>
#include <vector>

// reading
char* LoadFile(const char* filename);
char* LoadCompleteFile(const char* filename, long* length);
char* ReadRawBytesFromFile(const char* filename);

// reading by file type
unsigned char* LoadWaveFileData(const char* filePath, unsigned long& bufferSize, unsigned short& channels, unsigned int& frequency, unsigned short& bitsPerSample);

void WriteRawBytesToFile(const char* filename, const char* data, int numBytes);
void WriteStringToFile(const char* filename, const char* string);

bool FileExists(const char* filePath);
std::vector<std::string> ReadDir(const char* directoryPath); // #TODO Review name. Could involve "list" like ListDirFiles(path)

bool CreateUniqueFile(const char* filename);

const char* FindFileName(const char* filePath, bool includeExtension=true);
const char* FindFileExtension(const char* filePath);

std::string GetFileExtension(const char* filePath);
std::string GetFileNameWithExt(const char* filePath);
std::string GetFileNameNoExt(const char* filePath);
