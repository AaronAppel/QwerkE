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

#ifdef _QSTB_IMAGE
#include "Libraries/stb_image/stb_image.h"
#endif

#include "QC_Guid.h"

#include "QF_Files.h"
#include "QF_Mesh.h"
#include "QF_Meshes.h"
#include "QF_Paths.h"
#include "QF_Scenes.h"
#include "QF_Shader.h"
#include "QF_Sound.h"
#include "QF_Texture.h"
#include "QF_RendererHelpers.h"
// #include "QF_Scene.h" #TODO Add scenes to asset registry
#include "QF_Serialize.h"

namespace QwerkE {

	// #TODO How does the registry work without the editor, for the Game solution? Simply hide extra functions?

	static bool s_Initialized = false;

	// #TODO https://stackoverflow.com/questions/38955940/how-to-concatenate-static-strings-at-compile-time
	const char* const s_AssetsRegistryFileName = "Assets.qreg"; // Files::Extensions::Registry
	static std::unordered_map<size_t, AssetsList> s_AssetGuidToFileRegistry;

	std::unordered_map<size_t, AssetsMap> Assets::m_MapOfLoadedAssetMaps;
	std::unordered_map<size_t, AssetsMap> Assets::m_MapOfNullAssetMaps;

	GUID Assets::LoadAsset(const size_t typeId, const GUID& guid)
	{
		ASSERT(m_MapOfLoadedAssetMaps.find(typeId) == m_MapOfLoadedAssetMaps.end() ||
			m_MapOfLoadedAssetMaps[typeId].find(guid) == m_MapOfLoadedAssetMaps[typeId].end(), "Entry already exists!");

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
						if (Files::Exists(Paths::MeshBin(fileName.c_str()).c_str()))
						{
							// #TODO Deprecate bgfxFramework::Mesh
							bgfxFramework::Mesh* bgfxMesh = myMeshLoad(Paths::MeshBin(fileName.c_str()).c_str());

							Mesh* meshFromFile = new Mesh();
							meshFromFile->m_ibh = bgfxMesh->m_groups[0].m_ibh;
							meshFromFile->m_vbh = bgfxMesh->m_groups[0].m_vbh;

							// #TODO ASSERT(!Has<QwerkE::Mesh>(guid), "Mesh with GUID {0} already exists!", guid);
							m_MapOfLoadedAssetMaps[typeId][guid] = meshFromFile;

							// #TODO Free remaining memory from bgfxFramework::Mesh
						}
						else if (strcmp(fileName.c_str(), "Cube") == 0)
						{
							Mesh* meshFromCode = Meshes::CreateCube();
							m_MapOfLoadedAssetMaps[typeId][guid] = meshFromCode;
						}
						else if (strcmp(fileName.c_str(), "Plane") == 0)
						{
							Mesh* meshFromCode = Meshes::CreatePlane();
							m_MapOfLoadedAssetMaps[typeId][guid] = meshFromCode;
						}
						else if (strcmp(fileName.c_str(), "Triangle") == 0)
						{
							Mesh* meshFromCode = Meshes::CreateTriangle();
							m_MapOfLoadedAssetMaps[typeId][guid] = meshFromCode;
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

						Shader* newShader = new Shader(Paths::ShaderBin(vertexFileName.c_str()).c_str(),
							Paths::ShaderBin(fragmentFileName.c_str()).c_str(), guid);
						m_MapOfLoadedAssetMaps[typeId][guid] = newShader;
					}
					break;

				case Mirror::IdForType<Texture>():
					{
						ASSERT(!Has<Texture>(guid), "Texture with GUID {0} already exists!", guid);

						std::string textureFilePath = Paths::Texture(fileName.c_str());
						if (Files::Exists(textureFilePath.c_str()))
						{
							Texture* newTexture = new Texture(textureFilePath.c_str(), guid);
							m_MapOfLoadedAssetMaps[Mirror::IdForType<Texture>()][newTexture->Guid()] = newTexture;
						}
						else
						{
							LOG_ERROR("Could not find texture file: {0}!", fileName);
						}
					}
					break;

				case Mirror::IdForType<Sound>():
					{
						ASSERT(!Has<Sound>(guid), "Sound with GUID {0} already exists!", guid);

						std::string soundFilePath = Paths::Sound(fileName.c_str());
						if (Files::Exists(soundFilePath.c_str()))
						{
							Sound* newSound = new Sound(soundFilePath.c_str(), guid);
							m_MapOfLoadedAssetMaps[Mirror::IdForType<Sound>()][newSound->Guid()] = newSound;
						}
						else
						{
							LOG_ERROR("Could not find sound file: {0}!", fileName);
						}
					}
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
		ASSERT(!s_Initialized, "Assets:: already initialized!");

		// #TODO #NOTE TypeIds shouldn't be stored in data as they can change in code, between run times
		// #TODO Handle types that have multiple components, like shaders and materials

		Serialize::FromFile(Paths::Setting(s_AssetsRegistryFileName).c_str(), s_AssetGuidToFileRegistry);

		// Default Mesh entry
			// #TODO Switch to using Meshes:: functions
			// Mesh* nullMesh = Meshes::NewSquareMesh();
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

		// #TODO Clean up
		Meshes::Initialize(); // #TODO Move somewhere better, like rendering code

		// Default Shader entry
		// #TODO Use coded data instead of relying on a file to exist
		Shader* nullShader = new Shader(Paths::NullAsset("null_shader.vert.bin").c_str(), Paths::NullAsset("null_shader.frag.bin").c_str(), GUID::Invalid);
		ASSERT(nullShader, "Could not load null shader!");
		m_MapOfNullAssetMaps[Mirror::IdForType<Shader>()][nullShader->Guid()] = nullShader;

		Texture* nullTexture = new Texture(Paths::NullAsset("null_texture.png").c_str(), GUID::Invalid);
		ASSERT(nullTexture, "Could not load null texture!");
		m_MapOfNullAssetMaps[Mirror::IdForType<Texture>()][nullTexture->Guid()] = nullTexture;

		// #TODO Pass scene file path to allow loading scenes in other directories
		Scene* nullScene = new Scene("Empty.qscene", GUID::Invalid);// Scenes::CreateSceneFromFile(Paths::Scene("Empty.qscene"));
		ASSERT(nullScene, "Could not load null scene!");
		m_MapOfNullAssetMaps[Mirror::IdForType<Scene>()][GUID::Invalid] = nullScene;

		Sound* nullSound = new Sound(Paths::NullAsset("null_sound.wav").c_str(), 0);
		ASSERT(nullSound, "Could not load null sound!");
		m_MapOfNullAssetMaps[Mirror::IdForType<Sound>()][GUID::Invalid] = nullSound;

		s_Initialized = true;
	}

	void Assets::Shutdown()
	{
		SaveRegistry(); // #TODO Expose as option in editor/framework settings to auto save on exit

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

						case Mirror::IdForType<Texture>():
							delete static_cast<Texture*>(guidVoidPtrPair.second); break;

						case Mirror::IdForType<Sound>():
							delete static_cast<Sound*>(guidVoidPtrPair.second); break;

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
		s_Initialized = false;
	}

	bool Assets::Has(const size_t typeId, GUID guid)
	{
		if (m_MapOfLoadedAssetMaps.find(typeId) == m_MapOfLoadedAssetMaps.end())
			return false;

		return m_MapOfLoadedAssetMaps[typeId].find(guid) != m_MapOfLoadedAssetMaps[typeId].end();
	}

	std::string Assets::GetRegistryAssetFileName(const size_t typeId, const GUID guid)
	{
		const AssetsList& assetsRegistry = Assets::GetRegistryAssetList(typeId);
		for (size_t i = 0; i < assetsRegistry.size(); i++)
		{
			auto guidStringPair = assetsRegistry[i];
			if (guid == guidStringPair.first)
			{
				// #TODO Decide how to search for shaders and materials that have more than 1 string in vector
				return guidStringPair.second[0];
			}
		}
		return std::string("");
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

	bool Assets::ExistsInRegistry(const size_t mirrorTypeId, const GUID& guid)
	{
		// #TODO Review using type name strings instead of id values as are considered unstable ids should not be written to data
		auto& vectorGuidStrings = s_AssetGuidToFileRegistry[mirrorTypeId];
		for (size_t i = 0; i < vectorGuidStrings.size(); i++)
		{
			const std::pair<GUID, std::vector<std::string>>& pair = vectorGuidStrings[i];
			// #TODO Decide how to search for shader and materials that have more than 1 string in vector
			// #TODO ShaderComponenent should solve this issue
			constexpr int index = 0;
			if (pair.first == guid)
			{
				LOG_TRACE("{0} Asset exists in registry ({1}, {2})", __FUNCTION__, std::to_string(pair.first).c_str(), pair.second[index].c_str());
				return true;
			}
		}
		return false;
	}

	void Assets::AddToRegistry(const size_t mirrorTypeId, const GUID& guid, const std::string& fileName) // #TODO Take in std::vector<string> to support shader and materials
	{
		// #TODO More error checking
		if (!ExistsInRegistry(mirrorTypeId, guid, fileName))
		{
			auto& vectorGuidStrings = s_AssetGuidToFileRegistry[mirrorTypeId];
			vectorGuidStrings.push_back({ guid, { fileName } });
			LOG_INFO("{0} Added guid {1} to registry of type {2}", __FUNCTION__, guid, mirrorTypeId);
		}
	}

	void Assets::RemoveFromRegistry(const size_t mirrorTypeId, const GUID& guid)
	{
		// #TODO More error checking
		if (ExistsInRegistry(mirrorTypeId, guid))
		{
			auto& vectorGuidStrings = s_AssetGuidToFileRegistry[mirrorTypeId];
			for (size_t i = 0; i < vectorGuidStrings.size(); i++)
			{
				if (guid == vectorGuidStrings[i].first)
				{
					vectorGuidStrings.erase(vectorGuidStrings.begin() + i);
					LOG_INFO("{0} Removed guid {1} from registry of type {2}", __FUNCTION__, guid, mirrorTypeId);
					break;
				}
			}
		}
	}

	AssetsList& Assets::GetRegistryAssetList(const size_t assetListTypeId)
	{
		ASSERT(s_AssetGuidToFileRegistry.find(assetListTypeId) != s_AssetGuidToFileRegistry.end(), "Cannot return reference for registry asset list!");
		return s_AssetGuidToFileRegistry[assetListTypeId];
	}
#endif

}
