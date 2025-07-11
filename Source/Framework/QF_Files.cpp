#include "QF_Files.h"

#include <commdlg.h> // For file explorer dialogue
#include <filesystem> // For general file I/O
#include <stdio.h> // For fwrite, fclose, and general file I/O

#ifdef _QLODEPNG
#include "Libraries/lodepng/lodepng.h"
#endif

#include "QC_StringHelpers.h" // NumberAppendOrIncrement

#include "QF_Helpers.h" // NumberAppendOrIncrement
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

		Buffer LoadWav(const char* filePath)
		{
			Buffer buffer;

			const Path fileExtension = FileExtension(filePath);
			ASSERT(!fileExtension.compare(".wav"), "File missing .wav extension!");

#ifdef _QOPENAL
			buffer.Allocate();
			soundFile.s_Data = (char*)LoadWavFileData(filePath, soundFile.s_Size, soundFile.s_Channels, soundFile.s_Frequency, soundFile.s_BitsPerSample);
#else
			// #TODO Add checks if no supported audio library is loaded and asserts
			// error
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
