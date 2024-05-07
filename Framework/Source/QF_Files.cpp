#include "QF_Files.h"

#include <commdlg.h> // For file explorer dialogue
#include <filesystem> // For general file I/O
#include <stdio.h> // For fwrite, fclose, and general file I/O

#ifdef _QLODEPNG
#include "Libraries/lodepng/lodepng.h"
#endif

#include "QC_StringHelpers.h" // NumberAppendOrIncrement

#include "QF_Helpers.h" // NumberAppendOrIncrement
#include "QF_Window.h" //

namespace QwerkE {

    namespace Files {

		bool Exists(const char* filePath)
		{
			ASSERT(filePath, "Invalid filePath!");
			return std::filesystem::exists(filePath);
		}

		Buffer LoadFile(const char* filePath)
		{
			Buffer buffer;

			if (!filePath)
				return buffer;

			FILE* filehandle;
			const errno_t error = fopen_s(&filehandle, filePath, "rb");

			if (filehandle)
			{
				fseek(filehandle, 0, SEEK_END);
				const long fileSizeBytes = ftell(filehandle);
				rewind(filehandle);

				buffer.Allocate(fileSizeBytes);
				fread(buffer.Data(), fileSizeBytes, 1, filehandle);
				fclose(filehandle);
			}

			return buffer; // #TODO Buffer memory is released in destructor
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

		Buffer LoadWav(const char* filePath)
		{
			Buffer buffer;

			const Path fileExtension = FileExtension(filePath);
			ASSERT(!fileExtension.compare(".wav"), "File missing .wav extension!");

#ifdef _QOPENAL
			buffer.Allocate();
			soundFile.s_Data = (char*)LoadWavFileData(filePath, soundFile.s_Size, soundFile.s_Channels, soundFile.s_Frequency, soundFile.s_BitsPerSample);
#endif

			if (!buffer)
			{
				LOG_ERROR("{0} Error loading sound file!", __FUNCTION__);
			}
			return buffer;
		}

		Path FileName(const char* const filePath)
		{
			return Path(filePath).filename();
		}

		Path FileExtension(const char* filePath)
		{
			return Path(filePath).extension();
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

		std::string FileExplorer(DWORD flags, LPCSTR filter, bool save)
		{
			OPENFILENAMEA ofn;
			CHAR szFile[260] = { 0 };
			CHAR currentDir[256] = { 0 };
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			GLFWwindow* window = static_cast<GLFWwindow*>(Window::GetContext());
			ofn.hwndOwner = glfwGetWin32Window(window);
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			if (GetCurrentDirectoryA(256, currentDir))
				ofn.lpstrInitialDir = currentDir;
			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = flags;

			// Sets the default extension by extracting it from the filter
			ofn.lpstrDefExt = strchr(filter, '\0') + 1;

			if (save)
			{
				if (GetSaveFileNameA(&ofn) == TRUE)
					return ofn.lpstrFile;
			}
			else if (GetOpenFileNameA(&ofn) == TRUE)
				return ofn.lpstrFile;

			return std::string();
		}

		std::string ExplorerOpen(const char* filter)
		{
			// #TODO Add a starting directory argument to open up to a different directory than the current working directory
			// #TODO Look at arguments File description ("QwerkE Project"), and filterFile ("qproj" instead of (*.hproj)\0*.hproj\0)
			DWORD flags = OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST;
			return FileExplorer(flags, filter, false);
		}

		std::string ExplorerSave(const char* filter)
		{
			DWORD flags = OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;
			return FileExplorer(flags, filter, true);
		}

		Path UniqueFilePath(const char* const filePath)
		{
			const char* fullFileExtension = strrchr(filePath, '.');
			if (!fullFileExtension)
			{
				return Helpers::NumberAppendOrIncrement(filePath).get();
			}

			std::string fileNameNoExt = filePath;
			const u64 size = fullFileExtension - filePath; // Subtract pointer addresses
			const u8 extensionWithPeriodLength = strlen(filePath) - size;
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
