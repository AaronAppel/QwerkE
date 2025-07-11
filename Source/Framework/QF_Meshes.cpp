#include "QF_Meshes.h"

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

// #include "Libraries/bgfxFramework/include/bgfxFramework/SampleRenderData.h"

#include "QF_Mesh.h"

namespace QwerkE {

	namespace Meshes {

		// From bgfx examples project
		struct PosColorVertex_ // #TODO Review moving to a new file for organization
		{
			float x;
			float y;
			float z;
			uint32_t abgr;
			static void init()
			{
				ms_layout_
					.begin()
					.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
					.end();
			}
			static bgfx::VertexLayout ms_layout_;
		};
		bgfx::VertexLayout PosColorVertex_::ms_layout_;

		static PosColorVertex_ s_cubeVertices_[] =
		{
			{-1.0f,  1.0f,  1.0f, 0xff000000 },
			{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
			{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
			{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
			{-1.0f,  1.0f, -1.0f, 0xffff0000 },
			{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
			{-1.0f, -1.0f, -1.0f, 0xffffff00 },
			{ 1.0f, -1.0f, -1.0f, 0xffffffff },
		};

		// From bgfx example "Instancing"
		static const uint16_t s_cubeTriList_[36] = // #TODO Faces seem inverted so maybe change winding order to match other models
		{
			0, 1, 2, // 0
			1, 3, 2,
			4, 6, 5, // 2
			5, 6, 7,
			0, 2, 4, // 4
			4, 2, 6,
			1, 5, 3, // 6
			5, 7, 3,
			0, 4, 1, // 8
			4, 5, 1,
			2, 3, 6, // 10
			6, 3, 7,
		};
		// From bgfx example "Instancing"

		// From Example project  GameSolution
		struct VertexFormat_PosColor
		{
			vec3f pos;
			u32 color;

			static void InitVertexLayout()
			{
				format
					.begin()
					.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
					.end();
			};

			static bgfx::VertexLayout format;
		};
		bgfx::VertexLayout VertexFormat_PosColor::format;
		static const VertexFormat_PosColor g_TriangleVerts_[] =
		{
			{ vec3f(0.0f, 0.5f, 0.0f), 0xff0000ff }, // Top
			{ vec3f(-0.5f,-0.5f, 0.0f), 0xff00ff00 }, // Bottom-left
			{ vec3f(0.5f,-0.5f, 0.0f), 0xffff0000 }, // Bottom-right
		};
		static const u16 g_TriangleIndices_[] =
		{
			0, 2, 1,
		};
		struct VertexFormat_PosUV
		{
			vec3f pos;
			vec2f uv;

			static void InitVertexLayout()
			{
				format
					.begin()
					.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
					.end();
			};

			static bgfx::VertexLayout format;
		};
		bgfx::VertexLayout VertexFormat_PosUV::format;
		static const VertexFormat_PosUV g_SpriteVerts[] =
		{
			{ vec3f(-0.5f, 0.5f, 0.0f), vec2f(0,1) }, // Top-left
			{ vec3f(-0.5f,-0.5f, 0.0f), vec2f(0,0) }, // Bottom-left
			{ vec3f(0.5f,-0.5f, 0.0f), vec2f(1,0) }, // Bottom-right

			{ vec3f(-0.5f, 0.5f, 0.0f), vec2f(0,1) }, // Top-left
			{ vec3f(0.5f,-0.5f, 0.0f), vec2f(1,0) }, // Bottom-right
			{ vec3f(0.5f, 0.5f, 0.0f), vec2f(1,1) }, // Top-right
		};
		static const uint16_t g_SpriteIndices[] =
		{
			0,2,1, 3,5,4
		};

		void Initialize()
		{
			PosColorVertex_::init();
			VertexFormat_PosColor::InitVertexLayout();
			VertexFormat_PosUV::InitVertexLayout();
		}

		Mesh* CreateCube()
		{
			Mesh* newMesh = new Mesh();
			newMesh->m_vbh = bgfx::createVertexBuffer(
				bgfx::makeRef(s_cubeVertices_, sizeof(s_cubeVertices_)),
				PosColorVertex_::ms_layout_
			);
			newMesh->m_ibh = bgfx::createIndexBuffer(
				bgfx::makeRef(s_cubeTriList_, sizeof(s_cubeTriList_))
			);
			return newMesh;
		}

		Mesh* CreatePlane()
		{
			Mesh* newMesh = new Mesh();
			newMesh->m_vbh = bgfx::createVertexBuffer(
				bgfx::makeRef(g_SpriteVerts, sizeof(g_SpriteVerts)),
				VertexFormat_PosUV::format
			);
			newMesh->m_ibh = bgfx::createIndexBuffer(
				bgfx::makeRef(g_SpriteIndices, sizeof(g_SpriteIndices))
			);
			return newMesh;
		}

		Mesh* CreateTriangle()
		{
			Mesh* newMesh = new Mesh();
			newMesh->m_vbh = bgfx::createVertexBuffer(
				bgfx::makeRef(g_TriangleVerts_, sizeof(g_TriangleVerts_)),
				VertexFormat_PosColor::format
			);
			newMesh->m_ibh = bgfx::createIndexBuffer(
				bgfx::makeRef(g_TriangleIndices_, sizeof(g_TriangleIndices_))
			);
			return newMesh;
		}

	}

}
