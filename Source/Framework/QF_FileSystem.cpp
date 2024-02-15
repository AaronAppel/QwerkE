#include "QF_FileSystem.h"

#include <string>
#include <iostream>
#include <vector>

#pragma warning( disable : 26495 )
#include "Libraries/assimp/Importer.hpp"
#include "Libraries/assimp/scene.h"
#include "Libraries/assimp/postprocess.h"
#include "Libraries/assimp/config.h"
#include "Libraries/assimp/material.h"
#pragma warning( default : 26495 )

#pragma warning( disable : 28182 )
#pragma warning( disable : 6001 )
#pragma warning( disable : 6262 )
#include "Libraries/stb_image.h"
#pragma warning( default : 28182 )
#pragma warning( default : 6001 )
#pragma warning( default : 6262 )

#include "Libraries/lodepng/lodepng.h"
#include "Libraries/glew/GL/glew.h"
#include "Libraries/FlatheadGames/ImageHelpers.h"

#include "QF_Defines.h"

#ifdef OpenAL
#include "Libraries/OpenAL/include/al.h"
#include "Libraries/OpenAL/include/alc.h"
#endif

#include "QC_StringHelpers.h"

#include "QF_AssimpLoading.h"

#include "QF_FileSystem.h"
#include "QF_FileUtilities.h"
#include "QF_Resources.h"
#include "QF_Log.h"

#include "QF_Mesh.h"

namespace QwerkE {

	namespace File {

#ifdef OpenAL
		void CheckForOpenALErrors(const char* file, int line);
		ALuint OpenAL_LoadSound(const QSoundFile& soundFile);
#endif

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
#ifdef LODEPNG
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

#ifdef OpenAL
			soundHandle = OpenAL_LoadSound(soundFile);
#elif defined(XAudio)
			retValue = 0;
#else
#pragma error "Define audio library!"
#endif
			return soundHandle;
		}

		Mesh* LoadMeshInModelByName(const char* modelFilePath, const char* meshName)
		{
			if (false == Resources::MeshExists(meshName))
			{
				Mesh* mesh = nullptr;
#ifdef AI_CONFIG_H_INC
				Assimp::Importer importer;
				const aiScene* scene = importer.ReadFile(modelFilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

				if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
				{
					LOG_WARN("ERROR::ASSIMP::{0}", importer.GetErrorString());
					return nullptr;
				}
				_assimp_loadMeshByName(scene->mRootNode, scene, mesh, modelFilePath, meshName);
#else
#pragma error "Define model loading library!"
#endif
				Resources::AddMesh(meshName, mesh);
			}
			return Resources::GetMesh(meshName);
		}

		bool LoadModelFileToMeshes(const char* absoluteModelFilePath)
		{
			char* modelFullFileName = FullFileName(absoluteModelFilePath);
			if (!modelFullFileName)
				return false;

			if (Resources::MeshExists(modelFullFileName))
			{
				free(modelFullFileName);
				return false;
			}

			std::vector<Mesh*> meshes;
			std::vector<std::string> matNames;

#ifdef AI_CONFIG_H_INC
			Assimp::Importer importer;

			const aiScene* scene = importer.ReadFile(absoluteModelFilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				LOG_ERROR("ERROR::ASSIMP::{0}", importer.GetErrorString());
				return false;
			}
			_assimp_loadSceneNodeData(scene->mRootNode, scene, meshes, absoluteModelFilePath, matNames);
			// TODO: De-allocate RAM created by assimp
#else
#pragma error "Define model loading library!"
#endif

			for (size_t i = 0; i < meshes.size(); i++)
			{
				Resources::AddMesh(meshes[i]->GetFileName().c_str(), meshes[i]);
			}

			free(modelFullFileName);
			return true;
		}

#ifdef OpenAL
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

	}

}
