#include "QF_Assets.h"

#include <string>
#include <unordered_map>
#include <vector>

#ifdef _QBGFXFRAMEWORK
#include <bgfxFramework/SampleRenderData.h>
#include <bgfxFramework/LoadShader.h>
#include <bgfxFramework/bgfx_utils.h>
#endif

#ifdef _QMIRROR
#include "QF_Mirror.h"
#endif

#include "QF_Files.h"
#include "QF_Mesh.h"
#include "QF_Paths.h"
#include "QF_RendererHelpers.h"
#include "QF_Serialize.h"

namespace QwerkE {

	// #TODO How does the registry work without the editor, for the Game solution?

	std::unordered_map<size_t, AssetsMap> Assets::m_MapOfLoadedAssetMaps;

	Assets::ListFontPairs Assets::m_Fonts;
	Assets::ListMaterialPairs Assets::m_Materials;
	Assets::ListMeshPairs Assets::m_Meshes;
	Assets::ListShaders Assets::m_Shaders;
	Assets::ListTextures Assets::m_Textures;

	const char* const s_AssetsRegistryFileName = "Assets.qreg";
	static std::unordered_map<size_t, AssetsList> s_AssetGuidToFileRegistry;

	bool local_Load(const size_t mirrorTypeId, const char* filePath, const GUID& guid, std::unordered_map<size_t, AssetsMap>& mapOfAssetMaps);

	GUID Assets::LoadAsset(const size_t type, const GUID& guid)
	{
		AssetsList assetsForTypeInRegistry = s_AssetGuidToFileRegistry[type];
		for (size_t i = 0; i < assetsForTypeInRegistry.size(); i++)
		{
			std::pair<GUID, std::string> currentAssetListPair = assetsForTypeInRegistry[i];
			if (guid == currentAssetListPair.first)
			{
				std::string fileName = currentAssetListPair.second;
				std::string fileExtension = Files::FileExtension(fileName.c_str()).string();

				if (local_Load(type, Paths::Mesh(fileName.c_str()).c_str(), currentAssetListPair.first, m_MapOfLoadedAssetMaps))
				{
					return currentAssetListPair.first;
				}
				break;
			}
		}
		return GUID::Invalid;
	}

	void Assets::Initialize()
	{
		// #NOTE TypeIds shouldn't be stored in data as they can change in code, between run times

		// #TODO Handle types that have multiple components, like shaders and materials
		// m_MapOfLoadedAssetMaps[Mirror::TypeId<Shader>()][GUID()] = new std::pair<std::string, std::string>("vertex", "fragment");
		// m_MapOfLoadedAssetMaps[MirrorTypes::Material][GUID()] = new std::array<std::string, 11>( { "albedo", "diffuse", "normal", "specular", ...});

		Serialize::FromFile(Paths::Setting(s_AssetsRegistryFileName).c_str(), s_AssetGuidToFileRegistry);

		LoadAsset(Mirror::TypeId<bgfxFramework::Mesh>(), GUID::Invalid);
		ASSERT(Has<bgfxFramework::Mesh>(GUID::Invalid), "No null bgfxFramework::Mesh found!");

		{	// Default QwerkE::Mesh entry
			Mesh* nullMesh = new Mesh();
			nullMesh->m_vbh = bgfx::createVertexBuffer(
				bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), // Static data can be passed with bgfx::makeRef
				PosColorVertex::ms_layout
			);
			nullMesh->m_ibh = bgfx::createIndexBuffer(
				bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
			);
			nullMesh->m_GUID = GUID::Invalid;
			m_MapOfLoadedAssetMaps[Mirror::TypeId<Mesh>()][nullMesh->m_GUID] = nullMesh;
		}
		ASSERT(Has<Mesh>(GUID::Invalid), "No null QwerkE::Mesh found!");

		{	// Default Shader entry
			Shader* nullShader = new Shader();
			nullShader->m_Program = myLoadShaderProgram(
				Paths::Shader("vs_cubes.bin").c_str(),
				Paths::Shader("fs_cubes.bin").c_str()
			);
			nullShader->m_GUID = GUID::Invalid;
			m_MapOfLoadedAssetMaps[Mirror::TypeId<Shader>()][nullShader->m_GUID] = nullShader;

			Shader* shader1 = new Shader();
			shader1->m_Program = myLoadShaderProgram(
				Paths::Shader("vs_mesh.bin").c_str(),
				Paths::Shader("fs_mesh.bin").c_str()
			);
			m_MapOfLoadedAssetMaps[Mirror::TypeId<Shader>()][shader1->m_GUID] = shader1;

			// m_MapOfLoadedAssetMaps[Mirror::TypeId<Shader>()][1] = new std::pair<"vs_mesh.bin", "fs_mesh.bin">();
		}
		ASSERT(Has<Shader>(GUID::Invalid), "No null shader found!");
	}

	void Assets::Shutdown()
	{
		for (auto& mirrorTypeAssetMapPair : m_MapOfLoadedAssetMaps)
		{
			AssetsMap assetsMap = mirrorTypeAssetMapPair.second;

			for (auto& guidVoidPtrPair : assetsMap)
			{
				if (guidVoidPtrPair.second)
				{
					switch (mirrorTypeAssetMapPair.first)
					{
					case Mirror::TypeId<bgfxFramework::Mesh>():
						{
							// #TODO Fix bgfx break point on delete
							// bgfxFramework::Mesh* mesh = static_cast<bgfxFramework::Mesh*>(guidVoidPtrPair.second);
							// mesh->unload();
							// delete mesh;
						}
						break;
					case Mirror::TypeId<Mesh>():
						delete static_cast<Mesh*>(guidVoidPtrPair.second);
						break;

					case Mirror::TypeId<Shader>():
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
		m_MapOfLoadedAssetMaps.clear();

		// #TODO ASSERT all assets were properly released
	}

	bool local_Load(const size_t mirrorTypeId, const char* filePath, const GUID& guid, std::unordered_map<size_t, AssetsMap>& mapOfAssetMaps)
	{
		switch (mirrorTypeId)
		{
		case Mirror::TypeId<bgfxFramework::Mesh>():
			mapOfAssetMaps[mirrorTypeId][guid] = myMeshLoad(filePath); break;

		case Mirror::TypeId<Mesh>():
			{
				Mesh* nullMesh = new Mesh();
				nullMesh->m_vbh = bgfx::createVertexBuffer(
					bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), // Static data can be passed with bgfx::makeRef
					PosColorVertex::ms_layout
				);
				nullMesh->m_ibh = bgfx::createIndexBuffer(
					bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
				);
				nullMesh->m_GUID = guid;
				mapOfAssetMaps[mirrorTypeId][guid] = nullMesh;
			}
			break;

		default:
			return false;
		}
		return true;
	}

#ifndef _QRETAIL
	std::unordered_map<size_t, AssetsList>& Assets::ViewRegistry()
	{
		return s_AssetGuidToFileRegistry;
	}

	void Assets::SaveRegistry()
	{
		Serialize::ToFile(s_AssetGuidToFileRegistry, Paths::Setting(s_AssetsRegistryFileName).c_str());
	}

	bool Assets::ExistsInRegistry(const size_t mirrorTypeId, const GUID& guid, const std::string& fileName)
	{
		auto& vectorGuidStrings = s_AssetGuidToFileRegistry[mirrorTypeId];
		for (size_t i = 0; i < vectorGuidStrings.size(); i++)
		{
			std::pair<GUID, std::string>& pair = vectorGuidStrings[i];
			if (pair.first == guid || pair.second == fileName)
			{
				LOG_WARN("{0} Asset exists in registry ({1}, {2})", __FUNCTION__, std::to_string(pair.first).c_str(), pair.second.c_str());
				return true;
			}
		}
		return false;
	}

	void Assets::AddToRegistry(const size_t mirrorTypeId, const GUID& guid, const std::string& fileName)
	{
		if (!ExistsInRegistry(mirrorTypeId, guid, fileName))
		{
			auto& vectorGuidStrings = s_AssetGuidToFileRegistry[mirrorTypeId];
			vectorGuidStrings.push_back({ guid, fileName });
		}
	}

	AssetsList& GetRegistryAssetList(const size_t assetListTypeId)
	{
		if (s_AssetGuidToFileRegistry.find(assetListTypeId) != s_AssetGuidToFileRegistry.end())
		{
			return s_AssetGuidToFileRegistry[assetListTypeId];
		}
		ASSERT(false, "Cannot return reference for registry asset list!");
		return s_AssetGuidToFileRegistry[assetListTypeId];
	}
#endif

}
