#include "QF_Files.h"

#include <filesystem>

#ifdef _QLODEPNG
#include "Libraries/lodepng/lodepng.h"
#endif

#include "QC_StringHelpers.h" // NumberAppendOrIncrement

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
#endif

			if (!buffer)
			{
				LOG_ERROR("Error loading sound file!");
			}
			return buffer;
		}

		Path FileName(const char* filePath)
		{
			return Path().filename();
		}

		Path FileExtension(const char* filePath)
		{
			return Path().extension();
		}

		uPtr<char> UniqueFileName(const char* const filePath)
		{
			const char* periodChar = strrchr(filePath, '.');
			if (!periodChar)
			{
				uPtr<char> ptr = NumberAppendOrIncrement(filePath);
				return ptr;
			}

			std::string fileNameNoExt = filePath;
			const u64 size = periodChar - filePath;
			fileNameNoExt.resize(size);

			fileNameNoExt = NumberAppendOrIncrement(fileNameNoExt.c_str());
			uPtr<char> ptr = std::make_unique<char>(_strdup(fileNameNoExt.c_str()));
			return ptr;
		}

    }

}
