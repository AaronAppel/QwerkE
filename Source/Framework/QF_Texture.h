#pragma once

#include <string>

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

#ifdef _QSTB_IMAGE
#include "Libraries/stb_image/stb_image.h"
#endif

#include "QC_GUID.h"

#include "QF_Buffer.h"
#include "QF_Files.h"
#include "QF_Paths.h"

namespace QwerkE {

	class Texture
	{
	public:
		Texture() = default;

		Texture(float a_WindowWidth, float a_WindowHeight, bool a_HasMipMaps, uint16_t a_NumLayers,
			bgfx::TextureFormat::Enum a_TextureFormat, uint64_t a_Flags, GUID a_Guid = GUID()) :
			m_GUID(a_Guid)
		{
			Init(a_WindowWidth, a_WindowHeight, a_HasMipMaps, a_NumLayers, a_TextureFormat, a_Flags);
		}

		Texture(const char* a_TextureFilePath, GUID a_Guid = GUID()) :
			m_GUID(a_Guid)
		{
			Init(a_TextureFilePath);
		}

		Texture::~Texture()
		{
			Unload();
		}

		void Init(float a_WindowWidth, float a_WindowHeight, bool a_HasMipMaps, uint16_t a_NumLayers,
			bgfx::TextureFormat::Enum a_TextureFormat, uint64_t a_Flags)
		{
			m_TextureHandle = bgfx::createTexture2D(a_WindowWidth, a_WindowHeight, a_HasMipMaps, a_NumLayers, a_TextureFormat, a_Flags);
		}

		void Init(const char* a_TextureFilePath)
		{
			// #TODO Handle reloadable textures and reloading textures?

			Buffer imageFileBuffer = Files::LoadFile(a_TextureFilePath);

			// Have stb_image decompress png from memory into a raw color array.
			int width;
			int height;
			int channels;

			stbi_set_flip_vertically_on_load(true);
			unsigned char* pixelsBuffer = stbi_load_from_memory(imageFileBuffer.As<unsigned char>(), imageFileBuffer.SizeInBytes(), &width, &height, &channels, 4);
			ASSERT(pixelsBuffer != nullptr, "Error loading texture!"); // #TODO Recovery if statement early exit instead of asserting

			m_Format = bgfx::TextureFormat::Unknown;
			m_Size = vec2u32(width, height);
			m_HasMips = false;
			m_NumLayers = 1;
			m_Flags = 0;

			bgfx::TextureFormat::Enum bgfxFormat = bgfx::TextureFormat::RGBA8;
			unsigned int bufferSize = 0;

			switch (bgfxFormat)
			{
			case bgfx::TextureFormat::RG8:
			{
				bgfxFormat = bgfx::TextureFormat::RGB8;
				bufferSize = width * height * 3;
			}
			break;
			case bgfx::TextureFormat::RGBA8:
			{
				bgfxFormat = bgfx::TextureFormat::RGBA8;
				bufferSize = width * height * 4;
			}
			break;
			}

			m_TextureHandle = bgfx::createTexture2D(m_Size.x, m_Size.y, m_HasMips, m_NumLayers, bgfxFormat, m_Flags, bgfx::copy(pixelsBuffer, bufferSize));
			stbi_image_free(pixelsBuffer);
		}

		void Unload()
		{
			if (bgfx::isValid(m_TextureHandle))
			{
				bgfx::destroy(m_TextureHandle);
				m_TextureHandle = BGFX_INVALID_HANDLE;
			}
		}

		const bgfx::TextureHandle& TextureHandle() { return m_TextureHandle; }
		const GUID& Guid() { return m_GUID; }

	private:
		// void Reload()
		// {
		//     m_textureHandle = bgfx::createTexture2D(width, height, false, 1, bgfxFormat, 0, nullptr);
		//     bgfx::updateTexture2D(m_textureHandle, 0, 0, 0, 0, width, height, bgfx::copy(pixels, bufferSize));
		// }

		bgfx::TextureHandle m_TextureHandle = BGFX_INVALID_HANDLE;

		std::string m_FileName = "";

		// Buffer m_FileBuffer; // #TODO bgfx defers instantiation so buffers can't be stack scoped to Load(). Look at avoiding holding extra RAM

		GUID m_GUID = GUID::Invalid;

		bgfx::TextureFormat::Enum m_Format = bgfx::TextureFormat::Unknown;
		vec2u32 m_Size = vec2u32(0, 0);
		bool m_HasMips = false;
		u16 m_NumLayers = 1;
		u64 m_Flags = 0;
	};


}
