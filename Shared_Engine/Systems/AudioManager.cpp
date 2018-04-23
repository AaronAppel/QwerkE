#include "AudioManager.h"
#include "../../Shared_Generic/Utilities/PrintFunctions.h"

// OpenAL Reference: https://www.openal.org/documentation/OpenAL_Programmers_Guide.pdf
// wav loading: https://blog.csdn.net/u011417605/article/details/49662535
// const char* test_wav = "../../Shared_Generic/Resources/Sounds/Test.wav";

#include "../../Shared_Generic/Libraries/openal-soft/al.h"
#include "../../Shared_Generic/Libraries/openal-soft/alc.h"
#include "../../Shared_Generic/Libraries/openal-soft/alext.h"
#include "../../Shared_Generic/Libraries/openal-soft/efx.h"
#include "../../Shared_Generic/Libraries/openal-soft/efx.h"
#include "../../Shared_Generic/Libraries/openal-soft/efx-presets.h"

AudioManager::AudioManager()
{
    // Initialization
    // int InitAL(char ***argv, int *argc)
    char ***argv = 0;
    int *argc = 0;

    const ALCchar *name;
    ALCdevice *device;
    ALCcontext *ctx;

    /* Open and initialize a device */
    device = NULL;
    if (argc && argv && *argc > 1 && strcmp((*argv)[0], "-device") == 0)
    {
        device = alcOpenDevice((*argv)[1]);
        if (!device)
            fprintf(stderr, "Failed to open \"%s\", trying default\n", (*argv)[1]);
        (*argv) += 2;
        (*argc) -= 2;
    }
    if (!device)
        device = alcOpenDevice(NULL);
    if (!device)
    {
        fprintf(stderr, "Could not open a device!\n");
        //return 1;
    }

    ctx = alcCreateContext(device, NULL);
    if (ctx == NULL || alcMakeContextCurrent(ctx) == ALC_FALSE)
    {
        if (ctx != NULL)
            alcDestroyContext(ctx);
        alcCloseDevice(device);
        fprintf(stderr, "Could not set a context!\n");
        //return 1;
    }

    name = NULL;
    if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
        name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
    if (!name || alcGetError(device) != AL_NO_ERROR)
        name = alcGetString(device, ALC_DEVICE_SPECIFIER);
    printf("Opened \"%s\"\n", name);

   // return 0;
    /*
    Device = alcOpenDevice(NULL); // select the "preferred device"

    if (Device) {
        Context = alcCreateContext(Device, NULL);
        alcMakeContextCurrent(Context);
    }
    // Check for EAX 2.0 support
    g_bEAX = alIsExtensionPresent("EAX2.0");
    // Generate Buffers
    alGetError(); // clear error code
    alGenBuffers(NUM_BUFFERS, g_Buffers);
    ALenum error;
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        // DisplayALError("alGenBuffers :", error);
        return;
    }
    // Load test.wav
    loadWAVFile(test_wav, &format, &data, &size, &freq, &loop);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        // DisplayALError("alutLoadWAVFile test.wav : ", error);
        alDeleteBuffers(NUM_BUFFERS, g_Buffers);
        return;
    }
    // Copy test.wav data into AL Buffer 0
    alBufferData(g_Buffers[0], format, data, size, freq);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        // DisplayALError("alBufferData buffer 0 : ", error);
        alDeleteBuffers(NUM_BUFFERS, g_Buffers);
        return;
    }
    */
}

AudioManager::~AudioManager()
{
    /*
    // Unload test.wav
    unloadWAV(format, data, size, freq);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        // DisplayALError("alutUnloadWAV : ", error);
        alDeleteBuffers(NUM_BUFFERS, g_Buffers);
        return;
    }
    // Exit
    Context = alcGetCurrentContext();
    Device = alcGetContextsDevice(Context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(Context);
    alcCloseDevice(Device);
    */
}

void AudioManager::PlaySound()
{
    ConsolePrint("\nSound requested!");
}

/*
// Generate Sources
alGenSources(1,source);
if ((error = alGetError()) != AL_NO_ERROR)
{
DisplayALError("alGenSources 1 : ", error);
return;
}
// Attach buffer 0 to source
- 10 -
alSourcei(source[0], AL_BUFFER, g_Buffers[0]);
if ((error = alGetError()) != AL_NO_ERROR)
{
DisplayALError("alSourcei AL_BUFFER 0 : ", error);
}
*/