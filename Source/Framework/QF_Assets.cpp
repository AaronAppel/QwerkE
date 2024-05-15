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
	static std::unordered_map<MirrorTypes, AssetsList> s_AssetGuidToFileRegistry;

	using ListFontPairs = std::vector<std::pair<GUID, std::string>>;
	constexpr u16 materialsListSize = 11;
	using ListMaterialPairs = std::vector<std::pair<GUID, std::array<std::string, materialsListSize>>>;
	using ListMeshPairs = std::vector<std::pair<GUID, std::string>>;
	using ListShaders = std::vector<std::pair<GUID, std::pair<std::string, std::string>>>;
	using ListTextures = std::vector<std::pair<GUID, std::string>>;

	static ListFontPairs s_Fonts;
	static ListMaterialPairs s_Materials;
	static ListMeshPairs s_Meshes;
	static ListShaders s_Shaders;
	static ListTextures s_Textures;

	void local_Load(const MirrorTypes mirrorType, const char* filePath, const GUID& guid, std::unordered_map<MirrorTypes, AssetsMap>& mapOfAssetMaps);

	void Assets::Initialize()
	{
		// std::vector<std::pair<GUID, std::string>> vec = { { GUID(0), "cube.bin" }, { GUID(11140931355107737998), "bunny.bin" }, { GUID(8229536646138328776), "column.bin" } };
		// s_AssetGuidToFileRegistry[MirrorTypes::BgfxMesh] = vec;

		// s_AssetGuidToFileRegistry.insert({ MirrorTypes::BgfxMesh, { { GUID(0), "cube.bin" } } });
		// s_AssetGuidToFileRegistry.insert({ MirrorTypes::BgfxMesh, { { GUID(11140931355107737998), "bunny.bin" } } });
		// s_AssetGuidToFileRegistry.insert({ MirrorTypes::BgfxMesh, { { GUID(8229536646138328776), "column.bin" } } });
		// Serialize::ToFile(s_AssetGuidToFileRegistry, Paths::Setting(s_AssetsRegistryFileName).c_str());

		Serialize::FromFile(Paths::Setting(s_AssetsRegistryFileName).c_str(), s_AssetGuidToFileRegistry);

		for (auto& mirrorTypeVec : s_AssetGuidToFileRegistry)
		{
			MirrorTypes mirrorType = mirrorTypeVec.first;
			for (size_t i = 0; i < mirrorTypeVec.second.size(); i++)
			{
				auto guidString = mirrorTypeVec.second[i];
				std::string fileName = guidString.second;
				std::string fileExtension = Files::FileExtension(fileName.c_str()).string();

				local_Load(mirrorType, Paths::Mesh(fileName.c_str()).c_str(), guidString.first, m_MapOfAssetMaps);
			}
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
			if (true)
			{
				Shader* nullShader = new Shader();
				nullShader->m_Program = myLoadShaderProgram(
					Paths::Shader("vs_cubes.bin").c_str(),
					Paths::Shader("fs_cubes.bin").c_str()
				);
				nullShader->m_GUID = GUID::Invalid;
				m_MapOfAssetMaps[MirrorTypes::Shader][nullShader->m_GUID] = nullShader;
			}
			else
			{
				Shader* nullShader = new Shader();
				nullShader->m_Program = myLoadShaderProgram(
					Paths::Shader("vs_mesh.bin").c_str(),
					Paths::Shader("fs_mesh.bin").c_str()
				);
				nullShader->m_GUID = GUID::Invalid;
				m_MapOfAssetMaps[MirrorTypes::Shader][nullShader->m_GUID] = nullShader;
			}
		}

		ASSERT(Has<bgfxFramework::Mesh>(0), "No null mesh found!");
		ASSERT(Has<Shader>(0), "No null shader found!");
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
					case MirrorTypes::BgfxMesh:
						{
							// #TODO Fix bgfx break point on delete
							// bgfxFramework::Mesh* mesh = static_cast<bgfxFramework::Mesh*>(guidVoidPtrPair.second);
							// mesh->unload();
							// delete mesh;
						}
						break;
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

	void local_Load(const MirrorTypes mirrorType, const char* filePath, const GUID& guid, std::unordered_map<MirrorTypes, AssetsMap>& mapOfAssetMaps)
	{
		switch (mirrorType)
		{
		case MirrorTypes::BgfxMesh:
			mapOfAssetMaps[MirrorTypes::BgfxMesh][guid] = myMeshLoad(filePath); break;

		case MirrorTypes::Mesh:
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
				mapOfAssetMaps[MirrorTypes::Mesh][guid] = nullMesh;
			}
			break;

		default:
			break;
		}
	}

#ifndef _QRETAIL
	std::unordered_map<MirrorTypes, AssetsList>& Assets::ViewRegistry()
	{
		return s_AssetGuidToFileRegistry;
	}

	void Assets::SaveRegistry()
	{
		Serialize::ToFile(s_AssetGuidToFileRegistry, s_AssetsRegistryFileName); // #TODO Improve hard coded file name
	}

	void Assets::ExistsInRegistry(const GUID& guid, const std::string& fileName)
	{
		// #TODO Implement
	}

	void Assets::AddToRegistry(const MirrorTypes mirrorType, const GUID& guid, const std::string& fileName)
	{
		// #TODO if (!ExistsInRegistry(giod, fileName))
		auto& vectorGuidStrings = s_AssetGuidToFileRegistry[mirrorType];
		for (size_t i = 0; i < vectorGuidStrings.size(); i++)
		{
			std::pair<GUID, std::string>& pair = vectorGuidStrings[i];
			if (pair.first == guid || pair.second == fileName)
			{
				LOG_WARN("{0} Asset exists in registry ({1}, {2})", __FUNCTION__, std::to_string(pair.first).c_str(), pair.second.c_str());
				return;
			}
		}
		vectorGuidStrings.push_back({ guid, fileName });
	}
#endif

}
