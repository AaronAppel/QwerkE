#include "QF_Sound.h"

#ifdef _QOPENAL
#define AL_LIBTYPE_STATIC
#include "Libraries/openal-soft/include/AL/al.h"
#endif

namespace QwerkE {

    Sound::Sound(const char* a_SoundFilePath, GUID a_Guid) :
		m_Guid(a_Guid)
    {
		// #TODO Rewrite from old, deprecated format/style
        SoundFile* soundFile = Files::LoadWav(a_SoundFilePath);
		if (nullptr == soundFile)
			return;

		if (soundFile->channels == 1)
		{
			if (soundFile->bitsPerSample == 16)
				m_Format = AL_FORMAT_MONO16;
			else if (soundFile->bitsPerSample == 8)
				m_Format = AL_FORMAT_MONO8;
			else
			{
				LOG_ERROR("OpenAL_LoadSound(): Invalid bits per sample in file {0}", a_SoundFilePath);
				return;
			}
		}
		else if (soundFile->channels == 2)
		{
			if (soundFile->bitsPerSample == 16)
				m_Format = AL_FORMAT_STEREO16;
			else if (soundFile->bitsPerSample == 8)
				m_Format = AL_FORMAT_STEREO8;
			else
			{
				LOG_ERROR("OpenAL_LoadSound(): Invalid bits per sample in file {0}", a_SoundFilePath);
				return;
			}
		}
		else
		{
			LOG_ERROR("OpenAL_LoadSound(): Unsupported number of channels in file {0}", a_SoundFilePath);
			return;
		}

		alGenBuffers(1, &m_SoundBufferHandle);
		alBufferData(m_SoundBufferHandle, m_Format, soundFile->fileData, soundFile->bufferSize, soundFile->frequency);

		delete soundFile;
    }

    Sound::~Sound()
    {
		alDeleteBuffers(1, &m_SoundBufferHandle);
    }

    void Sound::Play(ALuint a_SourceHandle)
    {
		// if (a_SourceHandle) // #TODO Validate source

        alSourcei(a_SourceHandle, AL_LOOPING, AL_FALSE);
        alSourcei(a_SourceHandle, AL_BUFFER, m_SoundBufferHandle);
        alSourcePlay(a_SourceHandle);
    }

}
