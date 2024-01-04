#include "oal_Helpers.h"

#include "../../Debug/Log/Log.h"

namespace QwerkE {

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

    // #TODO Look at rewriting rewrite sourced code
    // Source : https://ffainelli.github.io/openal-example/
    std::string list_audio_devices(const ALCchar* devices)
    {
        // TODO: Improve device detection and assignment
        const ALCchar* device = devices, * next = devices + 1;
        std::string retValue = device;
        size_t len = 0;

        LOG_TRACE("OpenAL devices list:");
        LOG_TRACE("----------");
        while (device && *device != '\0' && next && *next != '\0') {
            LOG_TRACE("{0}", (char*)device);
            len = strlen(device);
            device += (len + 1);
            next += (len + 2);
        }
        LOG_TRACE("----------");
        return retValue;
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

}
