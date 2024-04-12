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
#include "QF_Serialization.h"
#include "QF_Shader.h"

namespace QwerkE {

	std::unordered_map<MirrorTypes, AssetsMap> Assets::m_MapOfAssetMaps;

	const char* const s_AssetsRegistryFileName = "Assets.qreg";
	static std::vector<std::pair<GUID, std::string>> s_AssetGuidToFileRegistry;

	void Assets::Initialize()
	{
		// #TODO Need to serialize as field, not as a complex type
		Serialization::DeserializeObjectFromFile(Paths::Settings(s_AssetsRegistryFileName).c_str(), s_AssetGuidToFileRegistry);

		{
			Mesh* nullMesh = new Mesh();
			nullMesh->m_vbh = bgfx::createVertexBuffer(
				bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), // Static data can be passed with bgfx::makeRef
				PosColorVertex::ms_layout
			);
			nullMesh->m_ibh = bgfx::createIndexBuffer(
				bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
			);
			m_MapOfAssetMaps[MirrorTypes::Mesh][GUID::Invalid] = nullMesh;
		}

		{
			Shader* nullShader = new Shader();
			nullShader->m_Program = myLoadShaderProgram("vs_cubes.bin", "fs_cubes.bin");
			m_MapOfAssetMaps[MirrorTypes::Shader][GUID::Invalid] = nullShader;
		}
	}

	void Assets::Shutdown()
	{
		SaveRegistry();

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

		// #TODO ASSERT all assets were properly released
	}

	void Assets::SaveRegistry()
	{
		Serialization::SerializeObjectToFile(s_AssetGuidToFileRegistry, Paths::Settings(s_AssetsRegistryFileName).c_str());
	}

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

	std::vector<std::pair<GUID, std::string>>& Assets::ViewRegistry()
	{
		return s_AssetGuidToFileRegistry;
	}

}
