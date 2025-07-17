#include "QF_Audio.h"

#include <string>
#include <vector>

#ifdef _QOPENAL
#define AL_LIBTYPE_STATIC
#include "Libraries/openal-soft/include/AL/al.h"
#include "Libraries/openal-soft/include/AL/alc.h"
#endif

#include "QF_Assets.h"
// #include "QF_FileSystem.h"
#include "QF_Log.h"
#include "QF_Sound.h"

namespace QwerkE {

    // #TODO Reference helpers in utils\openal-info.c

    void CheckForOpenALErrors(const char* file, int line) // #TODO Exists in openal-info.c checkALErrors() and checkALCErrors(), plus printALCInfo()
    {
        // switch (alGetError())
        // {
        //     // case AL_INVALID_DEVICE:
        //     // case AL_INVALID_CONTEXT:
        // case AL_INVALID_NAME:
        //     LOG_ERROR("AL_INVALID_NAME");
        //     break;
        // case AL_INVALID_ENUM:
        //     LOG_ERROR("AL_INVALID_ENUM");
        //     break;
        // case AL_INVALID_VALUE:
        //     LOG_ERROR("AL_INVALID_VALUE");
        //     break;
        // case AL_OUT_OF_MEMORY:
        //     LOG_ERROR("AL_OUT_OF_MEMORY");
        //     break;
        // default:
        //     LOG_ERROR("alGetError: Unknown error caught in file {0}({1})", file, line);
        //     break;
        // }
    }

    // TODO: Source this or rewrite
    // std::string list_audio_devices() // #TODO Exists in openal-info.c printDeviceList()
    std::string list_audio_devices(const ALCchar* devices)
    {
        std::string retValue = "";
        // TODO: Improve device detection and assignment
        const ALCchar* device = devices, * next = devices + 1;
        retValue = device;
        size_t len = 0;

        LOG_INFO("OpenAL devices list:");
        LOG_INFO("----------");
        while (device && *device != '\0' && next && *next != '\0') {
            LOG_INFO("{0}", (char*)device);
            len = strlen(device);
            device += (len + 1);
            next += (len + 2);
        }
        LOG_INFO("----------");
        return retValue;
    }

    namespace Audio {

        ALCdevice* m_Device = nullptr;
        ALCcontext* m_Context = nullptr;

        // ALboolean g_bEAX; // Why?

        // ALboolean loop = AL_FALSE;
        Sound* s_Sound = nullptr;

        // #TODO Abstract OpenAL to some "QF_AudioOpenAL.cpp" file
        class OpenALAudioManager;
        std::vector<OpenALAudioManager*> s_AudioManagers;
        u8 s_MainAudioManagerIndex = 0; // #TODO Improve support for multiple "AudioWorld"s

        bool s_Initialized = false;

        void Initialize()
        {
            ASSERT(!s_Initialized, "Audio:: is already initialized!");

            const ALCchar* devices = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
            std::string deviceName = list_audio_devices(devices);
            m_Device = alcOpenDevice(deviceName.c_str());

            if (!m_Device)
            {
            	LOG_ERROR("Error initializing audio device! OpenAL error code {0}", alGetError());
            	// return false;
            }

            m_Context = alcCreateContext(m_Device, NULL); // #TODO there is a print in alc code that would be nice to avoid
            if (!m_Context)
            {
            	LOG_ERROR("Error initializing audio context! OpenAL error code {0}", alGetError());
            	// return false;
            }
            alcMakeContextCurrent(m_Context);

            Assets::Load<Sound>(0);

            LOG_TRACE("OpenAL loaded successfully");

#ifdef _QOPENAL
            // s_AudioManagers.push_back(new OpenALAudioManager());
#endif
            s_Initialized = true; // m_AudioManager->Initialize()

            SetListenerOrientation(vec3f(0, 0, 0), vec3f(0, 0, 0)); // #TODO Review listener orientation set
        }

        void Shutdown()
        {
            ASSERT(s_Initialized, "Audio:: not yet initialized!");

            // m_Context = alcGetCurrentContext();
            // m_Device = alcGetContextsDevice(Context);
            alcMakeContextCurrent(NULL);
            alcDestroyContext(m_Context);
            alcCloseDevice(m_Device);

            for (size_t i = 0; i < s_AudioManagers.size(); i++)
            {
                // delete s_AudioManagers[i];
            }
            s_AudioManagers.clear();
            s_Initialized = false;
        }

        void PlaySound(const char* name) // #TODO Maybe not worth exposing? Force getting and playing Sound*?
        {
            ASSERT(s_Initialized, "Audio:: not yet initialized!");

            // Sound* sound = Assets::Get<Sound>("Id");
            // sound->Play();

            // #TODO Could be a case for DEBUG_ONLY_CHECK() to catch uninitialized calls without need for if checking?
            // s_AudioManagers[s_MainAudioManagerIndex]->PlaySound(name);
            LOG_TRACE("Audio.PlaySound()");
        }

        void SetListenerOrientation(vec3f a_Position, vec3f a_Velocity)
        {
            ASSERT(s_Initialized, "Audio:: not yet initialized!");

            // #TODO Create an AudioListener() class/struct
            alListenerf(AL_GAIN, 0.5f);
            alListener3f(AL_POSITION, a_Position.x, a_Position.y, a_Position.z);
            alListener3f(AL_VELOCITY, a_Velocity.x, a_Velocity.y, a_Velocity.z);
            ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
            alListenerfv(AL_ORIENTATION, listenerOri);

            // s_AudioManagers[s_MainAudioManagerIndex]->SetListenerOrientation(a_Position, a_Velocity);
            // #TODO vec3f expansion helper/macro
            LOG_TRACE("Audio.SetListenerOrientation(({0},{1},{2}), ({3,4,5}))", a_Position.x, a_Position.y, a_Position.z, a_Velocity.x, a_Velocity.y, a_Velocity.z);
        }

    }

}
