#include "QF_LoadImageFile.h"

#include "QF_FileUtilities.h"
#include "QF_Log.h"

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

namespace QwerkE {

    unsigned char* QwerkE_stb_image_loadImage(const char* path, unsigned int* imageWidth, unsigned int* imageHeight, GLenum& channels, bool flipVertically)
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

    unsigned char* QwerkE_lodepng_loadImage(const char* path, unsigned int* imageWidth, unsigned int* imageHeight, GLenum& channels, bool flipVertically)
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

}
