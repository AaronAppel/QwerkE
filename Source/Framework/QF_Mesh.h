#pragma once

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

namespace QwerkE {

	class Mesh
	{
	public:
		Mesh(GUID a_Guid = GUID())
		{
			m_GUID = a_Guid;
		}

		Mesh::~Mesh()
		{
			// #TODO Review bgfxFramework::Mesh::unload();
			// #TODO Not deleting bgfx data properly could throw bgfx allocator assert when restarting editor

			if (bgfx::isValid(m_vbh))
			{
				bgfx::destroy(m_vbh);
			}

			if (bgfx::isValid(m_ibh))
			{
				bgfx::destroy(m_ibh);
			}
		}

		void PreDrawSetup()
		{
			constexpr uint8_t stream = 0;
			bgfx::setVertexBuffer(stream, m_vbh);
			bgfx::setIndexBuffer(m_ibh);
		}

		// private: // #TODO Hide as members?
		bgfx::VertexBufferHandle m_vbh = BGFX_INVALID_HANDLE;
		bgfx::IndexBufferHandle m_ibh = BGFX_INVALID_HANDLE;
		GUID m_GUID = GUID::Invalid;
	};


}
