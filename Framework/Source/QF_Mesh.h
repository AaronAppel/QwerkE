#pragma once

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

namespace QwerkE {

	class Mesh
	{
	public:
		Mesh::Mesh()
		{
			// #TODO Should take in buffer GUIDs?
		}

		Mesh::~Mesh()
		{
			bgfx::destroy(m_vbh);
			bgfx::destroy(m_ibh);
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
