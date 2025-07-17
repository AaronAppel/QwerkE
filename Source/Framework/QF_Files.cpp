#include "QF_Files.h"

#include <commdlg.h> // For file explorer dialogue
#include <filesystem> // For general file I/O
#include <stdio.h> // For fwrite, fclose, and general file I/O

#ifdef _QLODEPNG
#include "Libraries/lodepng/lodepng.h"
#endif

#include "QC_StringHelpers.h" // NumberAppendOrIncrement

#include "QF_Helpers.h" // NumberAppendOrIncrement
#include "QF_Sound.h"
#include "QF_Window.h" // For Window::GetContext()

namespace QwerkE {

    namespace Files {

		bool Exists(const char* filePath)
		{
			ASSERT(filePath, "Invalid filePath!");
			return std::filesystem::status(filePath).type() == std::filesystem::file_type::regular;
		}

		Buffer LoadFile(const char* filePath) // #TODO Can I optimize return? Maybe using a reference instead?
		{
			// #TODO Validate adding null terminating character
			constexpr u8 addNullTerminatingChar = 1;

			Buffer buffer;

			if (!filePath)
				return buffer;

			FILE* filehandle;
			const errno_t error = fopen_s(&filehandle, filePath, "rb");

			if (filehandle)
			{
				fseek(filehandle, 0, SEEK_END);
				const u32 fileSizeBytes = ftell(filehandle);
				rewind(filehandle);

				buffer.Allocate(fileSizeBytes + addNullTerminatingChar);
				fread(buffer.Data(), fileSizeBytes, 1, filehandle);
				fclose(filehandle);

				if (addNullTerminatingChar)
				{
					buffer.Data()[buffer.SizeInBytes() - addNullTerminatingChar] = '\0';
				}
			}

			// #TODO Buffer memory is released in destructor
			// #TODO Assignment of return value copies to a new value using operator=() constructor
			return buffer;
		}

		Buffer LoadPng(const char* filePath, unsigned int* imageWidth, unsigned int* imageHeight, u16& channels)
		{
			const Path fileExtension = FileExtension(filePath);
			ASSERT(!fileExtension.compare(".png"), "File missing .png extension!");

			channels = 0x1908; // GL_RGBA;

			Buffer fileBuffer = Files::LoadFile(filePath);
			Buffer returnBuffer = Buffer(fileBuffer.SizeInBytes());

			if (fileBuffer)
			{
				unsigned char* bufferData = returnBuffer.As<unsigned char>();
				const u32 errorCode = lodepng_decode32(&bufferData, imageWidth, imageHeight, fileBuffer.Data(), fileBuffer.SizeInBytes());
				if (errorCode)
				{
					returnBuffer.Release();
				}
			}

			return returnBuffer;
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

		SoundFile* LoadWav(const char* filePath)
		{
			const Path fileExtension = FileExtension(filePath);
			ASSERT(!fileExtension.compare(".wav"), "File missing .wav extension!");

			SoundFile* soundFile = nullptr;

#ifdef _QOPENAL
			Buffer soundFileBuffer;

			soundFile = new SoundFile();
			soundFileBuffer = Files::LoadFile(filePath);
			soundFile->fileData = LoadWavFileData(filePath, soundFile->bufferSize, soundFile->channels, soundFile->frequency, soundFile->bitsPerSample);
#else
			// #TODO Add checks if no supported audio library is loaded and asserts
			// error
#endif

			if (!soundFileBuffer || !soundFile->fileData)
			{
				LOG_ERROR("{0} Error loading sound file!", __FUNCTION__);
			}
			return soundFile;
		}

		Path FileName(const char* const filePath)
		{
			return Path(filePath).filename();
		}

		Path FileNameNoExt(const char* const filePath)
		{
			return Path(filePath).stem();
		}

		Path FileExtension(const char* filePath)
		{
			return Path(filePath).extension();
		}

		void CreateEmptyFile(const char* const filePath)
		{
			if (!Exists(filePath))
			{
				FILE* filehandle;
				errno_t error = fopen_s(&filehandle, filePath, "w");
				if (!error && filehandle)
				{
					fclose(filehandle);
				}
				else
				{
					LOG_ERROR("{0} Could not create empty file {1}", __FUNCTION__, filePath);
				}
			}
		}

		void WriteStringToFile(const char* const fileData, const char* const filePath)
		{
			if (!filePath)
			{
				LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
				return;
			}

			FILE* filehandle;
			errno_t error = fopen_s(&filehandle, filePath, "w+");
			if (!error && filehandle)
			{
				fwrite(fileData, 1, strlen(fileData), filehandle);
				fclose(filehandle);
			}
			else
			{
				LOG_ERROR("{0} Could not open file for write {1}", __FUNCTION__, filePath);
			}
		}

		std::string FileExplorer(DWORD flags, LPCSTR filter, bool save, const char* relativeDirPath = nullptr)
		{
			// #TODO Select directories/folders, not just files: https://stackoverflow.com/questions/14596493/usage-of-getopenfilename-api-in-vc-for-opening-a-folder-not-a-file
			OPENFILENAMEA ofn;
			CHAR szFile[260] = { 0 };
			CHAR currentDir[256] = { 0 }; // #TODO Optional argument to starting directory
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			GLFWwindow* window = static_cast<GLFWwindow*>(Window::GetContext());
			ofn.hwndOwner = glfwGetWin32Window(window); // #TODO glfw call needs to be #ifdef wrapped and reviewed
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);

			if (relativeDirPath &&
				std::filesystem::status(relativeDirPath).type() == std::filesystem::file_type::directory)
			{
				ofn.lpstrInitialDir = relativeDirPath;
			}
			else
			{
				if (GetCurrentDirectoryA(256, currentDir))
					ofn.lpstrInitialDir = currentDir;
			}
			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = flags;

			// Sets the default extension by extracting it from the filter
			ofn.lpstrDefExt = strchr(filter, '\0') + 1;

			if (save)
			{
				if (TRUE == GetSaveFileNameA(&ofn))
				{
					return ofn.lpstrFile;
				}
			}
			else if (TRUE == GetOpenFileNameA(&ofn))
			{
				return ofn.lpstrFile;
			}

			return std::string();
		}

		std::string ExplorerOpen(const char* filter, const char* dirPath) // #TODO Supports both relative and absolute paths?
		{
			// #TODO Add a starting directory argument to open up to a different directory than the current working directory
			// #TODO Look at arguments File description ("QwerkE Project"), and filterFile ("qproj" instead of (*.hproj)\0*.hproj\0)
			DWORD flags = OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST;
			return FileExplorer(flags, filter, false, dirPath);
		}

		std::string ExplorerSave(const char* filter, const char* dirPath) // #TODO Supports both relative and absolute paths?
		{
			DWORD flags = OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;
			return FileExplorer(flags, filter, true, dirPath);
		}

		Path UniqueFilePath(const char* const filePath)
		{
			// #TODO Review logic
			Path path = filePath;
			std::string fileExt = path.has_extension() ? path.extension().string() : "";
			std::string fileNameNoExt = path.stem().string();
			while (Files::Exists(path.string().c_str()))
			{
				fileNameNoExt = NumberAppendOrIncrement(fileNameNoExt.c_str());
				path = path.parent_path() / (fileNameNoExt + fileExt);
			}
			return path.filename();

			const char* fullFileExtension = strrchr(filePath, '.');
			if (!fullFileExtension)
			{
				// return Helpers::NumberAppendOrIncrement(filePath).get();
				fullFileExtension = 0;
			}

			// std::string fileNameNoExt = filePath;
			const u64 size = fullFileExtension - filePath; // Subtract pointer addresses
			const u8 extensionWithPeriodLength = static_cast<u8>(strlen(filePath) - size);
			fileNameNoExt.resize(size);

			// #TODO Loop until confirmed unique
			while (Files::Exists((fileNameNoExt + fullFileExtension).c_str()))
			{
				fileNameNoExt = NumberAppendOrIncrement(fileNameNoExt.c_str());
			}
			fileNameNoExt += fullFileExtension;
			return fileNameNoExt.c_str();
		}

		Path UniqueFilePath(const char* const dirPath, const char* const fileName)
		{
			std::string filePath = dirPath;
			filePath += fileName;
			return UniqueFilePath(filePath.c_str());
		}

    }

}
