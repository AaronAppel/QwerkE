#pragma once

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

#include "QC_GUID.h"

#include "QF_Texture.h"

namespace QwerkE {

	class FrameBuffer
	{
	public:
		// #TODO Can hide constructor and friend with Assets class
		FrameBuffer() = default;

		// #TODO Decide how to load. If a Load() method makes sense. Using the constructor to assign an instance destroys the default instance
		FrameBuffer(u8 a_NumberOfTextures, Texture* a_Textures)
		{
			Init(a_NumberOfTextures, a_Textures);
		}

		FrameBuffer::~FrameBuffer()
		{
			Unload();
		}

		void Init(u8 a_NumberOfTextures, Texture* a_Textures)
		{
			bgfx::TextureHandle* textures = new bgfx::TextureHandle[a_NumberOfTextures];
			for (size_t i = 0; i < a_NumberOfTextures; i++)
			{
				textures[i] = a_Textures[i].TextureHandle();
			}
			m_FrameBufferHandle = bgfx::createFrameBuffer(a_NumberOfTextures, textures, false);
			delete[] textures;
		}

		void SetupView(bgfx::ViewId a_ViewId, const std::string& a_ViewName, uint16_t a_Flags,
			uint32_t a_ClearColorRgba = 0x000000ff, float a_DepthClear = 1.0f, uint8_t a_StencilClear = 0)
		{
			bgfx::setViewName(a_ViewId, a_ViewName.c_str());
			bgfx::setViewFrameBuffer(a_ViewId, m_FrameBufferHandle);
			bgfx::setViewClear(a_ViewId, a_Flags, a_ClearColorRgba, a_DepthClear, a_StencilClear);
		}

		void Unload()
		{
			if (bgfx::isValid(m_FrameBufferHandle))
			{
				bgfx::destroy(m_FrameBufferHandle);
				m_FrameBufferHandle = BGFX_INVALID_HANDLE;
			}
		}

		const bgfx::FrameBufferHandle& FrameBufferHandle() { return m_FrameBufferHandle; }
		const GUID& Guid() { return m_GUID; }

	private:
		// #TODO Can optionally own textures
		bgfx::FrameBufferHandle m_FrameBufferHandle = BGFX_INVALID_HANDLE;

		GUID m_GUID = GUID::Invalid;
	};


}
