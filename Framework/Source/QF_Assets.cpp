#include "QF_Assets.h"

#include <string>
#include <unordered_map>

#ifdef _QBGFXFRAMEWORK
#include <bgfxFramework/SampleRenderData.h>
#include <bgfxFramework/LoadShader.h>
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Mirror.h"
#endif

#include "QF_Files.h"
#include "QF_Mesh.h"
#include "QF_Shader.h"

namespace QwerkE {

	sPtr<Mesh> Assets::m_NullMesh = nullptr;
	std::unordered_map<GUID, sPtr<Mesh>> Assets::m_Meshes;

	using AssetsMap = std::unordered_map<GUID, void*>;
	std::unordered_map<MirrorTypes, AssetsMap> Assets::m_MapOfAssetMaps;

	// const sPtr<T> local_Load(const std::unordered_map<GUID, sPtr<T>>& assetMap, const GUID& guid);

	sPtr<Shader> Assets::m_NullShader = nullptr;
	bgfx::ProgramHandle Assets::m_ProgramCube;

	void Assets::Initialize()
	{
		m_NullMesh = std::make_shared<Mesh>();
		m_NullMesh->m_vbh = bgfx::createVertexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices))
			, PosColorVertex::ms_layout
		);

		m_NullMesh->m_ibh = bgfx::createIndexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
		);
		m_Meshes.insert({ m_NullMesh->m_GUID, m_NullMesh });

		m_MapOfAssetMaps[MirrorTypes::Mesh].insert({ GUID::Invalid, (void*)&m_NullMesh });

		m_NullShader = std::make_shared<Shader>();
		m_NullShader->m_Program = myLoadShaderProgram("vs_cubes.bin", "fs_cubes.bin");
		m_MapOfAssetMaps[MirrorTypes::Shader].insert({ GUID::Invalid, (void*)&m_NullShader });
	}

	void Assets::Shutdown()
	{
		m_Meshes.clear();
		m_NullMesh.reset();
		// #TODO ASSERT all meshes were released

		m_NullShader.reset();

		bgfx::destroy(m_ProgramCube);
	}

	// template <typename T>
	// void GetAsset(const char* assetName)
	// {
	// 	// Get extension
	// 	// For a given extension, check if file is loaded
	// 	// For a given extension, load if needed
	// 	// For a loaded asset, return asset
	// 	// Handle invalid (unloaded) asset case
	// 	return ;
	// }

	template <typename T>
	sPtr<T>& local_Load(const std::unordered_map<GUID, sPtr<T>>& assetMap, const GUID& guid)
	{
		ASSERT(textureFilePath, "Invalid textureFilePath!");

		Path fileName = Files::FileName(textureFilePath);
		// #TODO Make fileName.string().c_str() nicer to use
		if (local_Has(fileName.string().c_str()))
		{
#ifdef _QDEBUG
			LOG_WARN("{0} Asset already exists: {1}", __FUNCTION__, fileName.string().c_str());
#endif
			return *s_Textures[textureFilePath];
		}
		// #TODO Load asset
		return s_NullTexture;
	}

}
