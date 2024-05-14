#include "QF_Assets.h"

#include <string>
#include <unordered_map>

#ifdef _QBGFX
// #include <bgfx/examples/common/bgfx_utils.h>
#endif

#ifdef _QBGFXFRAMEWORK
#include <bgfxFramework/SampleRenderData.h>
#include <bgfxFramework/LoadShader.h>
// #include <bgfxFramework/bgfx_utils.h>
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Mirror.h"
#endif

#include "QF_Files.h"
#include "QF_Mesh.h"
#include "QF_Paths.h"
#include "QF_RendererHelpers.h"
#include "QF_Serialize.h"
#include "QF_Shader.h"

namespace QwerkE {

	std::unordered_map<MirrorTypes, AssetsMap> Assets::m_MapOfAssetMaps;

	const char* const s_AssetsRegistryFileName = "Assets.qreg";
	static std::vector<std::pair<GUID, std::string>> s_AssetGuidToFileRegistry;

	static std::vector<bgfxFramework::Mesh*> s_WrongMeshAssets;

	void Assets::Initialize()
	{
		Serialize::FromFile(Paths::Setting(s_AssetsRegistryFileName).c_str(), s_AssetGuidToFileRegistry);

		for (size_t i = 0; i < s_AssetGuidToFileRegistry.size(); i++)
		{
			// #TODO Load asset
			Mesh* newMesh = new Mesh();
			std::string filePath = Paths::Mesh(s_AssetGuidToFileRegistry[i].second.c_str()).c_str();
			bgfxFramework::Mesh* wrongMeshClass = myMeshLoad(filePath.c_str());
			s_WrongMeshAssets.push_back(wrongMeshClass);
			newMesh->m_ibh = wrongMeshClass->m_groups[0].m_ibh;
			newMesh->m_vbh = wrongMeshClass->m_groups[0].m_vbh;
			newMesh->m_GUID = s_AssetGuidToFileRegistry[i].first;
			m_MapOfAssetMaps[MirrorTypes::Mesh][newMesh->m_GUID] = newMesh;
		}

		{
			// bgfxFramework::Mesh* mesh = nullptr;
			// loadMesh(mesh, Paths::Mesh("bunny.bin").c_str(), false);
			// meshLoad();
		}

		{
			Mesh* nullMesh = new Mesh();
			nullMesh->m_vbh = bgfx::createVertexBuffer(
				bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), // Static data can be passed with bgfx::makeRef
				PosColorVertex::ms_layout
			);
			nullMesh->m_ibh = bgfx::createIndexBuffer(
				bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
			);
			nullMesh->m_GUID = GUID::Invalid;
			m_MapOfAssetMaps[MirrorTypes::Mesh][nullMesh->m_GUID] = nullMesh;
		}

		{
			Shader* nullShader = new Shader();
			nullShader->m_Program = myLoadShaderProgram(
				Paths::Shader("vs_cubes.bin").c_str(),
				Paths::Shader("fs_cubes.bin").c_str()
			);
			nullShader->m_GUID = GUID::Invalid;
			m_MapOfAssetMaps[MirrorTypes::Shader][nullShader->m_GUID] = nullShader;
		}
	}

	void Assets::Shutdown()
	{
		for (auto& mirrorTypeAssetMapPair : m_MapOfAssetMaps)
		{
			AssetsMap assetsMap = mirrorTypeAssetMapPair.second;

			for (auto& guidVoidPtrPair : assetsMap)
			{
				if (guidVoidPtrPair.second)
				{
					switch (mirrorTypeAssetMapPair.first)
					{
					case MirrorTypes::Mesh:
						delete static_cast<Mesh*>(guidVoidPtrPair.second);
						break;

					case MirrorTypes::Shader:
						delete static_cast<Shader*>(guidVoidPtrPair.second);
						break;

					default:
						LOG_CRITICAL("{0} Unsupported type!", __FUNCTION__);
						break;
					}
				}
			}
			assetsMap.clear();
		}
		m_MapOfAssetMaps.clear();

		for (size_t i = 0; i < s_WrongMeshAssets.size(); i++)
		{
			if (s_WrongMeshAssets[i])
			{
				// #TODO Fix bgfx break point on delete
				// s_WrongMeshAssets[i]->unload();
			}
		}
		s_WrongMeshAssets.clear();

		// #TODO ASSERT all assets were properly released
	}

	std::vector<std::pair<GUID, std::string>>& Assets::ViewRegistry()
	{
		return s_AssetGuidToFileRegistry;
	}

#ifndef _QRETAIL
	void Assets::SaveRegistry()
	{
		Serialize::ToFile(s_AssetGuidToFileRegistry, "NewSerializationWindow"); // #TODO Improve hard coded file name
	}
#endif

	void Assets::ExistsInRegistry(const GUID& guid, const std::string& fileName)
	{
		// #TODO Implement
	}

	void Assets::AddToRegistry(const GUID& guid, const std::string& fileName)
	{
		// #TODO if (!ExistsInRegistry(giod, fileName))

		for (size_t i = 0; i < s_AssetGuidToFileRegistry.size(); i++)
		{
			std::pair<GUID, std::string>& pair = s_AssetGuidToFileRegistry[i];
			if (pair.first == guid || pair.second == fileName)
			{
				LOG_WARN("{0} Asset exists in registry ({1}, {2})", __FUNCTION__, std::to_string(pair.first).c_str(), pair.second.c_str());
				return;
			}
		}
		s_AssetGuidToFileRegistry.push_back({ guid, fileName });
	}

}
