#include "QF_Files.h"

#ifdef _QLODEPNG
#include "Libraries/lodepng/lodepng.h"
#endif

namespace QwerkE {

    namespace Files {

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

		Buffer LoadPng(const char* filePath, unsigned int* imageWidth, unsigned int* imageHeight, GLenum& channels)
		{
			channels = GL_RGBA;

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

    }

}
