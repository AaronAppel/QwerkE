#pragma once

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

#include "Libraries/bgfxFramework/include/bgfxFramework/SampleRenderData.h"

#include "QF_Mesh.h"

namespace QwerkE {

	namespace Meshes {

		Mesh* NewSquareMesh()
		{
			Mesh* newMesh = new Mesh();
			newMesh->m_vbh = bgfx::createVertexBuffer(
				bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
				PosColorVertex::ms_layout
			);
			newMesh->m_ibh = bgfx::createIndexBuffer(
				bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
			);
			return newMesh;
		}

	}

}
