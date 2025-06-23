#pragma once

#include <string>

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

#include "QC_GUID.h"

#include "QF_Buffer.h"
// #include "QF_Files.h"
// #include "QF_Paths.h"

namespace QwerkE {

	class Texture
	{
	public:
		Texture() = default;

		// #TODO Can hide constructor and friend with Assets class
		Texture(float a_WindowWidth, float a_WindowHeight, bool a_HasMipMaps, uint16_t a_NumLayers,
			bgfx::TextureFormat::Enum a_TextureFormat, uint64_t a_Flags)
		{
			Init(a_WindowWidth, a_WindowHeight, a_HasMipMaps, a_NumLayers, a_TextureFormat, a_Flags);
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

		// #TODO Consider hiding (private) for release builds?
		void Reload()
		{
			// m_textureHandle = bgfx::createTexture2D(width, height, false, 1, bgfxFormat, 0, nullptr);
			// bgfx::updateTexture2D(m_textureHandle, 0, 0, 0, 0, width, height, bgfx::copy(pixels, bufferSize));
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
		bgfx::TextureHandle m_TextureHandle = BGFX_INVALID_HANDLE;

		std::string m_FileName = "";

		// Buffer m_FileBuffer; // #TODO bgfx defers instantiation so buffers can't be stack scoped to Load(). Look at avoiding holding extra RAM

		GUID m_GUID = GUID::Invalid;
	};


}
