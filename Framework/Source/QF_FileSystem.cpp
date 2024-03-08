#include "QF_FileSystem.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#ifdef _QSTBIMAGE
#pragma warning( disable : 28182 )
#pragma warning( disable : 6001 )
#pragma warning( disable : 6262 )
#define STB_IMAGE_IMPLEMENTATION
#include "Libraries/stb_image.h"
#pragma warning( default : 28182 )
#pragma warning( default : 6001 )
#pragma warning( default : 6262 )
#endif

#ifdef _QLODEPNG
#include "Libraries/lodepng/lodepng.h"
#endif

#ifdef _QGLEW
#include "Libraries/glew/glew.h"
#endif

#ifdef _QGLFW3
#include "Libraries/glfw/QC_glfw3.h"
#endif

#ifdef _QFLATHEAD
#include "Libraries/FlatheadGames/ImageHelpers.h"
#endif

// #include "QF_Defines.h"

#ifdef _QOPENAL
#include "Libraries/OpenAL/al.h"
#include "Libraries/OpenAL/alc.h"
#endif

#include "QC_StringHelpers.h"

#include "QF_FileSystem.h"
#include "QF_FileUtilities.h"
#include "QF_Log.h"
#include "QF_Window.h"

namespace QwerkE {

	namespace Files {

#ifdef _QOPENAL
		void CheckForOpenALErrors(const char* file, int line);
		ALuint OpenAL_LoadSound(const QSoundFile& soundFile);
#endif

#ifdef _QSTBIMAGE
		unsigned char* priv_QwerkE_stb_image_loadImage(const char* path, unsigned int* imageWidth, unsigned int* imageHeight, GLenum& channels, bool flipVertically)
		{
			unsigned char* pngbuffer = 0;
			int nrChannels = 0;

			// Note: must cast values to signed ints. Could lead to errors.
			// buffer file data
			stbi_set_flip_vertically_on_load(flipVertically);
			pngbuffer = stbi_load(path, (int*)&imageWidth, (int*)&imageHeight, &nrChannels, channels); // can set 0 to desired comp (rgba == 4)
			// if (pngbuffer == nullptr || strcmp((const char*)pngbuffer, "") == 0)
			if (!pngbuffer)
			{
				stbi_image_free(pngbuffer);
				return nullptr; // null handle
			}

			if (nrChannels == 3)
				channels = GL_RGB;
			else if (nrChannels == 4)
				channels = GL_RGBA;

			// cleanup
			//unsigned char* r = new unsigned char[sizeof(pngbuffer)];
			//memcpy_s(r, sizeof(r), pngbuffer, sizeof(pngbuffer));
			//stbi_image_free(pngbuffer);
			return pngbuffer;
		}
#endif

#ifdef _QLODEPNG
		unsigned char* priv_QwerkE_lodepng_loadImage(const char* path, unsigned int* imageWidth, unsigned int* imageHeight, GLenum& channels, bool flipVertically)
		{
			long filesize;
			unsigned char* pngbuffer = 0;
			unsigned int result = -1;

			const unsigned char* filebuffer = (unsigned char*)LoadCompleteFile(path, &filesize);

			if (filebuffer != nullptr)
			{
				result = lodepng_decode32(&pngbuffer, imageWidth, imageHeight, filebuffer, filesize);
			}

			if (result != 0)
			{
				free(pngbuffer);
				delete[] filebuffer;
				return nullptr;
			}

			channels = GL_RGBA;

			delete[] filebuffer;
			return pngbuffer;
		}
#endif

#ifdef _QOPENAL
		void priv_LoadSoundFileData(const char* filePath, QSoundFile& soundFile)
		{
			soundFile.s_FileName = FileName(filePath, true);
			soundFile.s_Extension = SmartFileExtension(filePath).get();

			if (strcmp(soundFile.s_Extension.c_str(), "wav") == 0)
			{
				soundFile.s_Data = (char*)LoadWavFileData(filePath, soundFile.s_Size, soundFile.s_Channels, soundFile.s_Frequency, soundFile.s_BitsPerSample);
			}
			else if (strcmp(soundFile.s_Extension.c_str(), "mp3") == 0)
			{
				// soundFile.s_Type = "mp3";
				// TODO: soundFile.s_Data = LoadMP3FileData(...);
			}

			if (soundFile.s_Data == nullptr)
			{
				LOG_ERROR("OpenAL error buffering sound data!");
			}
		}
#endif

		char* FullFileName(const char* filePath)
		{
			return FileName(filePath, true);
		}

		// determine image type and use proper library
		// lodepng, stb_image, SOIL, etc
		unsigned char* LoadImageFileData(const char* path, unsigned int* imageWidth, unsigned int* imageHeight, GLenum& channels, bool flipVertically)
		{
			unsigned char* returnBuffer = nullptr;

			std::string extention = SmartFileExtension(path).get();

			if (strcmp(extention.c_str(), "jpg") == 0)
			{
				LOG_ERROR("{0} .jpg image not loaded. No working jpg load function currently: {1}", __FUNCTION__, path);
			}
			else if (strcmp(extention.c_str(), "png") == 0)
			{
#ifdef _QLODEPNG
				returnBuffer = priv_QwerkE_lodepng_loadImage(path, imageWidth, imageHeight, channels, flipVertically);
#else
#pragma error "Define image loading library!"
#endif
			}
			else
			{
				LOG_WARN("{0} Image type unsupported for file: {1}", __FUNCTION__, path);
			}

			if (!returnBuffer)
			{
				LOG_ERROR("{0} Failed to load image: {1}", __FUNCTION__, path);
				return nullptr;
			}

			LOG_INFO("{0} Loaded: {1}", __FUNCTION__, path);
			return returnBuffer;
		}

		SoundHandle LoadSound(const char* soundPath)
		{
			SoundHandle soundHandle = 0;
			QSoundFile soundFile;
			priv_LoadSoundFileData(soundPath, soundFile);

#ifdef _QOPENAL
			soundHandle = OpenAL_LoadSound(soundFile);
#elif defined(XAudio)
			retValue = 0;
#else
#pragma error "Define audio library!"
#endif
			return soundHandle;
		}

#ifdef _QOPENAL
		void CheckForOpenALErrors(const char* file, int line)
		{
			const ALenum alErrorCode = alGetError();
			switch (alErrorCode)
			{
			case AL_NO_ERROR:
				break;
			case AL_INVALID_NAME: // Same as ALC_INVALID_DEVICE
				LOG_ERROR("AL_INVALID_NAME");
				break;
			case AL_INVALID_ENUM: // Same as ALC_INVALID_CONTEXT
				LOG_ERROR("AL_INVALID_ENUM");
				break;
			case AL_INVALID_VALUE:
				LOG_ERROR("AL_INVALID_VALUE");
				break;
			case AL_OUT_OF_MEMORY:
				LOG_ERROR("AL_OUT_OF_MEMORY");
				break;
			default:
				LOG_ERROR("alGetError: Unknown error caught in file {0}({1})", file, line);
				break;
			}
		}

		ALuint OpenAL_LoadSound(const QSoundFile& soundFile)
		{
			if (soundFile.s_Data == nullptr)
				return 0;

			ALuint retValue = 0;
			GLenum format = 0;

			if (soundFile.s_Channels == 1)
			{
				if (soundFile.s_BitsPerSample == 16)
					format = AL_FORMAT_MONO16;
				else if (soundFile.s_BitsPerSample == 8)
					format = AL_FORMAT_MONO8;
				else
				{
					LOG_ERROR("OpenAL_LoadSound(): Invalid bits per sample in file {0}", soundFile.s_FileName);
					return 0;
				}
			}
			else if (soundFile.s_Channels == 2)
			{
				if (soundFile.s_BitsPerSample == 16)
					format = AL_FORMAT_STEREO16;
				else if (soundFile.s_BitsPerSample == 8)
					format = AL_FORMAT_STEREO8;
				else
				{
					LOG_ERROR("OpenAL_LoadSound(): Invalid bits per sample in file {0}", soundFile.s_FileName);
					return 0;
				}
			}
			else
			{
				LOG_ERROR("OpenAL_LoadSound(): Unsupported number of channels in file {0}", soundFile.s_FileName);
				return 0;
			}

			alGenBuffers(1, &retValue);
			alBufferData(retValue, format, soundFile.s_Data, soundFile.s_Size, soundFile.s_Frequency);

			CheckForOpenALErrors(__FILE__, __LINE__);
			return retValue;
		}
#endif

#include <commdlg.h>

		// std::string filepath = FileDialogs::OpenFile("QwerkE Project (*.qproj)\0*.qproj\0");
		std::string ExplorerOpen(const char* filter)
		{
			OPENFILENAMEA ofn;
			CHAR szFile[260] = { 0 };
			CHAR currentDir[256] = { 0 };
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);

#ifdef _QGLFW3
			ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Window::GetContext());
#else
#error "Define window library!"
#endif
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			if (GetCurrentDirectoryA(256, currentDir))
				ofn.lpstrInitialDir = currentDir;
			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

			if (GetOpenFileNameA(&ofn) == TRUE)
				return ofn.lpstrFile;

			return std::string();

		}

		std::string ExplorerSave(const char* filter)
		{
			OPENFILENAMEA ofn;
			CHAR szFile[260] = { 0 };
			CHAR currentDir[256] = { 0 };
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
#ifdef _QGLFW3
			ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Window::GetContext());
#else
#error "Define window library!"
#endif
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			if (GetCurrentDirectoryA(256, currentDir))
				ofn.lpstrInitialDir = currentDir;
			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

			// Sets the default extension by extracting it from the filter
			ofn.lpstrDefExt = strchr(filter, '\0') + 1;

			if (GetSaveFileNameA(&ofn) == TRUE)
				return ofn.lpstrFile;

			return std::string();
		}

	}

}
