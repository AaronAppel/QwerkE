#pragma once

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

namespace QwerkE {

	class Mesh
	{
	public:
		Mesh::~Mesh()
		{
			if (true) // #TODO Validate if destroy is needed
			{
				bgfx::destroy(m_vbh);
				bgfx::destroy(m_ibh);
			}
		}

		void PreDrawSetup()
		{
			bgfx::setVertexBuffer(0, m_vbh);
			bgfx::setIndexBuffer(m_ibh);
		}

		// private: // #TODO Hide as members?
		bgfx::VertexBufferHandle m_vbh;
		bgfx::IndexBufferHandle m_ibh;
		GUID m_GUID;
	};


}
