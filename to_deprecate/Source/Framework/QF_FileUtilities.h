#pragma once

#include <string>
#include <memory>
#include <vector>

// #TODO Move/deprecate file utils to File:: namespace/struct
char* LoadFile(const char* fileName);
char* LoadCompleteFile(const char* fileName, long* length);
char* ReadRawBytesFromFile(const char* const fileName);

unsigned char* LoadWavFileData(const char* filePath, unsigned long& bufferSize, unsigned short& channels, unsigned int& frequency, unsigned short& bitsPerSample);

void WriteRawBytesToFile(const char* filename, const char* data, int numBytes);
void WriteStringToFile(const char* filename, const char* string);

char* UniqueFileNameNumberAppend(const char* fileDirectory, const char* fileName, const char* extension);
bool FileExists(const char* filePath);
std::unique_ptr<std::vector<std::string>> ReadDir(const char* directoryPath); // #TODO Review name. Could involve "list" like ListDirFiles(path)

std::unique_ptr<char> SmartFileName(const char* filePathOrName, bool includeExtension);
char* FileName(const char* filePath, bool includeExtension);
std::unique_ptr<char> SmartFileExtension(const char* filePathOrName);
char* FileExtension(const char* filePath);
