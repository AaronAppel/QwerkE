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
#include "QF_Texture.h"
#include "QF_RendererHelpers.h"
// #include "QF_Scene.h" #TODO Add scenes to asset registry
#include "QF_Serialize.h"

namespace QwerkE {

	// #TODO How does the registry work without the editor, for the Game solution? Simply hide extra functions?

	// #TODO https://stackoverflow.com/questions/38955940/how-to-concatenate-static-strings-at-compile-time
	const char* const s_AssetsRegistryFileName = "Assets.qreg"; // Files::Extensions::Registry
	static std::unordered_map<size_t, AssetsList> s_AssetGuidToFileRegistry;

	std::unordered_map<size_t, AssetsMap> Assets::m_MapOfLoadedAssetMaps;
	std::unordered_map<size_t, AssetsMap> Assets::m_MapOfNullAssetMaps;

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
				case Mirror::IdForType<Mesh>():
					{
						if (constexpr bool loadFromMeshFile = true)
						{
							// #TODO Deprecate bgfxFramework::Mesh
							bgfxFramework::Mesh* bgfxMesh = myMeshLoad(Paths::Mesh(fileName.c_str()).c_str());

							Mesh* meshFromFile = new Mesh();
							meshFromFile->m_ibh = bgfxMesh->m_groups[0].m_ibh;
							meshFromFile->m_vbh = bgfxMesh->m_groups[0].m_vbh;

							// #TODO ASSERT(!Has<QwerkE::Mesh>(guid), "Mesh with GUID {0} already exists!", guid);
							m_MapOfLoadedAssetMaps[typeId][guid] = meshFromFile;

							// #TODO Free remaining memory from bgfxFramework::Mesh
						}
						else // #TODO Create a null mesh from code (avoid serialization? Maybe keep it out of registry, but still show in assets lists UI)
						{
						}
					}
					break;

				case Mirror::IdForType<Scene>():
					// #NOTE Scene transition changes to Scenes::CreateSceneFromFile(fileName)
					ASSERT(!Has<Scene>(guid), "Scene with GUID {0} already exists!", guid);
					m_MapOfLoadedAssetMaps[typeId][guid] = Scenes::CreateSceneFromFile(Paths::Scene(fileName.c_str()));
					break;

				case Mirror::IdForType<Shader>():
					{
						ASSERT(!Has<Shader>(guid), "Shader with GUID {0} already exists!", guid);

						constexpr u8 vertexFileNameIndex = 0;
						const std::string& vertexFileName = currentAssetListPair.second[vertexFileNameIndex];
						constexpr u8 fragmentFileNameIndex = 1;
						const std::string& fragmentFileName = currentAssetListPair.second[fragmentFileNameIndex];

						Shader* newShader = new Shader(vertexFileName.c_str(), fragmentFileName.c_str(), guid);
						m_MapOfLoadedAssetMaps[typeId][guid] = newShader;
					}
					break;

				case Mirror::IdForType<Texture>():
					ASSERT(false, "Unimplemented!");
					// Texture newTexture = new Texture();
					// m_MapOfLoadedAssetMaps[typeId][guid] = newTexture;
					break;

				default:
					LOG_ERROR("Unsupported asset type {0}!", typeId);
					return GUID::Invalid;
				}

				AddToRegistry(typeId, guid, fileName); // #TODO Should always try to add to registry?
				return guid;
			}
		}
		LOG_WARN("Could not find registry entry for asset of type {0} and GUID {1}", typeId, guid);
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
				bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
				PosColorVertex::ms_layout
			);
			nullMesh->m_ibh = bgfx::createIndexBuffer(
				bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
			);
			nullMesh->m_GUID = GUID::Invalid;
			m_MapOfNullAssetMaps[Mirror::IdForType<Mesh>()][GUID::Invalid] = nullMesh;
		}

		{   // Default Shader entry
			// #TODO Use coded data instead of relying on a file to exist
			Shader* nullShader = new Shader("Cubes.vert.bin", "Cubes.frag.bin", GUID::Invalid);
			m_MapOfNullAssetMaps[Mirror::IdForType<Shader>()][nullShader->Guid()] = nullShader;
		}

		{
			// #TODO Null texture
		}
	}

	void Assets::Shutdown()
	{
		for (auto& assetsList : { m_MapOfLoadedAssetMaps, m_MapOfNullAssetMaps })
		{
			for (auto& mirrorTypeAssetMapPair : assetsList)
			{
				AssetsMap assetsMap = mirrorTypeAssetMapPair.second;

				for (auto& guidVoidPtrPair : assetsMap)
				{
					if (guidVoidPtrPair.second)
					{
						switch (mirrorTypeAssetMapPair.first)
						{
						case Mirror::IdForType<Mesh>():
							delete static_cast<Mesh*>(guidVoidPtrPair.second); break;

						case Mirror::IdForType<Shader>():
							delete static_cast<Shader*>(guidVoidPtrPair.second); break;

						case Mirror::IdForType<Scene>():
							// Scenes::Shutdown(); should already be called
							// #TODO If this gets hit, is that an error? Assert?
							break;

						default:
							LOG_CRITICAL("{0} Unsupported type!", __FUNCTION__);
							break;
						}
					}
				}
				assetsMap.clear();
			}
		}
		m_MapOfLoadedAssetMaps.clear();
		m_MapOfNullAssetMaps.clear();

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
				LOG_TRACE("{0} Asset exists in registry ({1}, {2})", __FUNCTION__, std::to_string(pair.first).c_str(), pair.second[index].c_str());
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
		// #TODO Create registry if it doesn't exist?
		ASSERT(s_AssetGuidToFileRegistry.find(assetListTypeId) != s_AssetGuidToFileRegistry.end(), "Cannot return reference for registry asset list!");
		return s_AssetGuidToFileRegistry[assetListTypeId];
	}
#endif

}
