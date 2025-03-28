#pragma once

#include <filesystem>
#include <string>

#include "QC_StringHelpers.h"

typedef unsigned int GLenum;

namespace QwerkE {

    class Mesh;

    typedef unsigned int SoundHandle;

    struct QFile
    {
        std::string s_FileName = Constants::gc_DefaultStringValue;
        unsigned long s_Size = 0;
        std::string s_Extension = Constants::gc_DefaultStringValue;
        char* s_Data = nullptr;

        virtual ~QFile() { if (s_Data) { delete s_Data; } } // #TODO Sometimes need free(s_Data)
    };

    struct QImageFile : public QFile
    {
        unsigned int s_Width = 0;
        unsigned int s_Height = 0;
        unsigned short s_Channels = 0;
    };

    struct QSoundFile : public QFile
    {
        unsigned short s_Channels = 0;
        unsigned int s_Frequency = 0;
        unsigned short s_BitsPerSample = 0;

        SoundHandle s_Handle = 0;

        // TODO: ~QSoundFile() alDeleteBuffers(1, &s_Handle)
    };

    namespace Files {

        std::string ExplorerOpen();
        std::string ExplorerSave();

        // TODO: Allow flags for loading images a certain way
        // TODO: Remove GLenum and any implementation specific variables, types, or styles
        // TODO: LoadHDRImage // https://learnopengl.com/PBR/IBL/Diffuse-irradiance
        // LoadImage is already a windows macro...

        char* FullFileName(const char* filePath); // #TODO Use smart pointer to de-allocate

        unsigned char* LoadImageFileData(const char* path, unsigned int* imageWidth, unsigned int* imageHeight, GLenum& channels, bool flipVertically = 0);

        SoundHandle LoadSound(const char* soundName);

        // TODO: Load scene from software like 3DS Max, Blender, etc
        // load things like lights, cameras and everything from 1 file
        // look at creating a conversion helper for QwerkE.scene to unity, blender, etc

        // Material* GetMaterialFromMatFile(const char* path);

    }

}
