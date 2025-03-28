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

#include "QC_Guid.h"

#include "QF_Files.h"
#include "QF_Mesh.h"
#include "QF_Paths.h"
#include "QF_Scenes.h"
#include "QF_Shader.h"
#include "QF_RendererHelpers.h"
// #include "QF_Scene.h" #TODO Add scenes to asset registry
#include "QF_Serialize.h"

namespace QwerkE {

	// #TODO How does the registry work without the editor, for the Game solution?

	std::unordered_map<size_t, AssetsMap> Assets::m_MapOfLoadedAssetMaps;

	// #TODO https://stackoverflow.com/questions/38955940/how-to-concatenate-static-strings-at-compile-time
	const char* const s_AssetsRegistryFileName = "Assets.qreg"; // Files::Extensions::Registry
	static std::unordered_map<size_t, AssetsList> s_AssetGuidToFileRegistry;

	GUID Assets::LoadAsset(const size_t typeId, const GUID& guid)
	{
		AssetsList assetsForTypeInRegistry = GetRegistryAssetList(typeId);
		for (size_t i = 0; i < assetsForTypeInRegistry.size(); i++)
		{
			std::pair<GUID, std::vector<std::string>> currentAssetListPair = assetsForTypeInRegistry[i];
			if (guid == currentAssetListPair.first)
			{
				// #TODO Decide how to search for shader and materials that have more than 1 string in vector
				constexpr int index = 0;
				const std::string& fileName = currentAssetListPair.second[index];

				// #NOTE Scene transition changes removes below line
				const std::string fileExtension = Files::FileExtension(fileName.c_str()).string();

				// #TODO Handle loading errors (return GUID::Invalid;)
				switch (typeId)
				{
				case Mirror::TypeId<bgfxFramework::Mesh>():
					m_MapOfLoadedAssetMaps[typeId][guid] = myMeshLoad(Paths::Mesh(fileName.c_str()).c_str()); break;

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
						m_MapOfLoadedAssetMaps[typeId][guid] = nullMesh;
					}
					break;

				case Mirror::TypeId<Scene>():
					// #NOTE Scene transition changes to Scenes::CreateSceneFromFile(fileName)
					m_MapOfLoadedAssetMaps[typeId][guid] = Scenes::CreateSceneFromFile(Paths::Scene(fileName.c_str())); break;

				case Mirror::TypeId<Shader>():
					{
						// #TODO Loading hard coded shader for now to test.
						// Need to setup shaders to have both vertex and fragment files linked together.
						// Might need to create shader component struct with 2 pointers and shaders just reference shader components.
						Shader* newShader = new Shader();
						newShader->m_Program = myLoadShaderProgram( // Blinn shading
							Paths::Shader("vs_mesh.bin").c_str(),
							Paths::Shader("fs_mesh.bin").c_str()
						);

						const bool error = false; // #TODO Catch shader creation error
						// newShader->m_Program != 0?
						if (error)
						{
							delete newShader;
						}
						else
						{
							m_MapOfLoadedAssetMaps[typeId][guid] = newShader;
						}
					}
					break;

				default:
					// #TODO LOG_ERROR("Unsupported asset type!");
					return GUID::Invalid;
				}

				AddToRegistry(typeId, guid, fileName); // #TODO Should always try to add to registry?
				return guid;
			}
		}
		return GUID::Invalid;
	}

	void Assets::Initialize()
	{
		// #TODO #NOTE TypeIds shouldn't be stored in data as they can change in code, between run times
		// #TODO Handle types that have multiple components, like shaders and materials

		Serialize::FromFile(Paths::Setting(s_AssetsRegistryFileName).c_str(), s_AssetGuidToFileRegistry);

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

		// ASSERT(!GetRegistryAssetList(Mirror::TypeId<Shader>()).empty(), "No null shader found!");
		// ASSERT(!GetRegistryAssetList(Mirror::TypeId<Mesh>()).empty(), "No null QwerkE::Mesh found!");
		// ASSERT(!GetRegistryAssetList(Mirror::TypeId<bgfxFramework::Mesh>()).empty(), "No null bgfxFramework::Mesh found!");
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
		// #TODO Review using type name strings instead of id values as are considered unstable ids should not be written to data
		auto& vectorGuidStrings = s_AssetGuidToFileRegistry[mirrorTypeId];
		for (size_t i = 0; i < vectorGuidStrings.size(); i++)
		{
			const std::pair<GUID, std::vector<std::string>>& pair = vectorGuidStrings[i];
			// #TODO Decide how to search for shader and materials that have more than 1 string in vector
			// #TODO ShaderComponenent should solve this issue
			constexpr int index = 0;
			if (pair.first == guid || pair.second[index] == fileName)
			{
				LOG_WARN("{0} Asset exists in registry ({1}, {2})", __FUNCTION__, std::to_string(pair.first).c_str(), pair.second[index].c_str());
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
			// #TODO Decide how to search for shader and materials that have more than 1 string in vector
			vectorGuidStrings.push_back({ guid, { fileName } });
		}
	}

	AssetsList& Assets::GetRegistryAssetList(const size_t assetListTypeId)
	{
		ASSERT(s_AssetGuidToFileRegistry.find(assetListTypeId) != s_AssetGuidToFileRegistry.end(), "Cannot return reference for registry asset list!");
		return s_AssetGuidToFileRegistry[assetListTypeId];
	}
#endif

}
