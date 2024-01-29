#include "QF_oal_AudioManager.h"

#include "QC_StringHelpers.h"

#include "QF_Defines.h"

#ifdef OpenAL
#include "Libraries/OpenAL/include/al.h"
#include "Libraries/OpenAL/include/alc.h"
#endif

#include "QF_Log.h"
#include "QF_FileSystem.h"
#include "QF_Resources.h"
#include "QF_AudioSource.h"

namespace QwerkE {

#ifdef OpenAL
	std::string list_audio_devices(const ALCchar* devices);
#endif

	bool OpenALAudioManager::Initialize()
	{
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
		return true;
	}

	OpenALAudioManager::~OpenALAudioManager()
	{
		Shutdown();
	}

	void OpenALAudioManager::Shutdown()
	{
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
	}

	void OpenALAudioManager::PlaySound(const char* name)
	{
		m_Source->Play(Resources::GetSound(name));
	}

	void OpenALAudioManager::SetListenerOrientation(vec3 position, vec3 velocity)
	{
		// TODO: Create an AudioListener() object or component/routine pair
		// that will remember and manipulate listener data.
		alListenerf(AL_GAIN, 0.5f);
		alListener3f(AL_POSITION, position.x, position.y, position.z);
		alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
		alListenerfv(AL_ORIENTATION, listenerOri);
	}

#ifdef OpenAL
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

}
