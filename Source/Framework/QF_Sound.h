#pragma once

// #include "Libraries/OpenAL/include/al.h"

#include "QC_GUID.h"

#include "QF_Files.h"

typedef unsigned int ALuint; // From OpenAL include/AL/al.h

namespace QwerkE {

    struct SoundFile
    {
        SoundFile() = default;
        SoundFile(unsigned long a_BufferSize, unsigned short a_Channels,
            unsigned int a_Frequency, unsigned short a_BitsPerSample, unsigned char* a_FileData = nullptr) :
            bufferSize(a_BufferSize),
            channels(a_Channels),
            frequency(a_Frequency),
            bitsPerSample(a_BitsPerSample),
            fileData(a_FileData)
        {
        }

        ~SoundFile()
        {
            if (fileData)
            {
                delete[] fileData;
            }
        }

        unsigned long bufferSize = 0;
        unsigned short channels = 0;
        unsigned int frequency = 0;
        unsigned short bitsPerSample = 0;
        unsigned char* fileData = nullptr;
    };

    class Sound // #TODO Becomes: class Sound
    {
    public:
        Sound() = default;
        Sound(const char* a_SoundFilePath, GUID a_Guid = GUID());
        ~Sound();

        // #TODO Abstract from OpenAL
        // void Play(ALuint buffer);
        void Play(ALuint a_SourceHandle);

        const GUID& Guid() { return m_Guid; }

        const ALuint& BufferHandle() { return m_SoundBufferHandle; }
        const GLenum& Format() { return m_Format; }

    private:
        // #TODO Abstract from OpenAL
        // ALuint m_SourceID = 0;

        ALuint m_SoundBufferHandle = 0;
        GLenum m_Format = 0;

        GUID m_Guid = GUID::Invalid;
    };

}
