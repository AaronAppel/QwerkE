#include "QF_Audio.h"

#include "QF_LibrariesLoad.h"

#include "QF_Log.h"

#ifdef _QOPENAL
#include "Libraries/OpenAL/al.h"
#include "Libraries/OpenAL/alc.h"
#else
#error "Define audio library!"
#endif

#include "QF_AudioSource.h"

namespace QwerkE {

    namespace Audio {

#ifdef _QOPENAL
        ALCdevice* m_Device = nullptr;
        ALCcontext* m_Context = nullptr;
        ALboolean loop = AL_FALSE;

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
#endif
        AudioSource* m_Source = nullptr;

        bool s_Initialized = false;

        bool Initialize()
        {
#ifdef _QOPENAL
            std::string deviceName = list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
            m_Device = alcOpenDevice(deviceName.c_str());

            if (!m_Device)
            {
                LOG_ERROR("Error initializing audio device! OpenAL error code {0}", alGetError());
                return false;
            }

            m_Context = alcCreateContext(m_Device, NULL); // #TODO there is a print in alc code that would be nice to avoid
            if (!m_Context)
            {
                LOG_ERROR("Error initializing audio context! OpenAL error code {0}", alGetError());
                return false;
            }
            alcMakeContextCurrent(m_Context);

            m_Source = new AudioSource();
            m_Source->SetOrientation(vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, 0, 0));
            SetListenerOrientation(vec3(0, 0, 0), vec3(0, 0, 0)); // #TODO Review listener orientation set

            LOG_TRACE("OpenAL loaded successfully");
            s_Initialized = true;
#endif
            return s_Initialized;
        }

        void Shutdown()
        {
#ifdef _QOPENAL
            delete m_Source;
            // TODO: cleanup openal
            /*
            alDeleteBuffers(NUM_BUFFERS, g_Buffers);
            Context = alcGetCurrentContext();
            Device = alcGetContextsDevice(Context);
            alcMakeContextCurrent(NULL);
            alcDestroyContext(Context);
            alcCloseDevice(Device);
            */
            s_Initialized = false;
#endif
        }

        void PlaySound(const char* name)
        {
            if (s_Initialized)
            {
#ifdef _QOPENAL
                // m_Source->Play(Assets::GetSound(name));
#endif
            }
            else
            {
                LOG_TRACE("Audio.PlaySound()");
            }
        }

        void SetListenerOrientation(vec3 position, vec3 velocity)
        {
            if (s_Initialized)
            {
#ifdef _QOPENAL
                // TODO: Create an AudioListener() object or component/routine pair
                // that will remember and manipulate listener data.
                alListenerf(AL_GAIN, 0.5f);
                alListener3f(AL_POSITION, position.x, position.y, position.z);
                alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
                ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
                alListenerfv(AL_ORIENTATION, listenerOri);
#endif
            }
            else
            {
                // #TODO vec3 expansion helper/macro
                LOG_TRACE("Audio.SetListenerOrientation(({0},{1},{2}), ({3,4,5}))", position.x, position.y, position.z, velocity.x, velocity.y, velocity.z);
            }
        }

        bool Initialized()
        {
            return s_Initialized;
        }

    }

}
