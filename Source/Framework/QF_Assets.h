#pragma once

#include <vector>

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Structs.h"
#endif

#include "QC_Guid.h"

namespace QwerkE {

	using AssetsMap = std::unordered_map<GUID, void*>; // #NOTE Smart pointers require compile time types. Could look at another solution
	using AssetsList = std::vector<std::pair<GUID, std::string>>;

	class Assets
	{
	public:
		Assets() = delete;

		static void Initialize();
		static void Shutdown();

		template <typename T>
		static bool Has(GUID guid)
		{
			const size_t typeEnum = Mirror::InfoForType<T>()->id;
			std::unordered_map<GUID, T*>* assetMap = (std::unordered_map<GUID, T*>*)&m_MapOfAssetMaps[typeEnum];

			return assetMap->find(guid) != assetMap->end();
		}

		template <typename T>
		static T* Get(GUID guid)
		{
			const size_t typeEnum = Mirror::InfoForType<T>()->id;
			std::unordered_map<GUID, T*>* assetMap = (std::unordered_map<GUID, T*>*)&m_MapOfAssetMaps[typeEnum];

			if (!Has<T>(guid))
			{
				guid = GUID::Invalid;
			}

			void* assetPtr = m_MapOfAssetMaps[typeEnum][guid];
			return static_cast<T*>(assetPtr);
		}

#ifndef _QRETAIL
		template <typename T>
		static const std::unordered_map<GUID, T*>& ViewAssets()
		{
			const size_t typeEnum = Mirror::InfoForType<T>()->id;
			std::unordered_map<GUID, T*>* assetMap = (std::unordered_map<GUID, T*>*)&m_MapOfAssetMaps[typeEnum];
			return *assetMap;
		}

		template <typename T>
		static std::unordered_map<size_t, AssetsList>& ViewRegistry()
		{
			const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();
			switch (typeInfo)
			{
			case Mirror::TypeId<Mesh>():
				// #TODO Can't return 1 type as each list is a different type
				// m_Meshes;
				break;
			default:
				// #TODO Error unsupported type
				break;
			}
		}

		static std::unordered_map<size_t, AssetsList>& ViewRegistry();
		static void SaveRegistry();

		static bool Assets::ExistsInRegistry(const size_t mirrorTypeId, const GUID& guid, const std::string& fileName);
		template <typename T>
		static bool ExistsInRegistry(const GUID& guid, const std::string& fileName)
		{
			return ExistsInRegistry(Mirror::TypeId<T>, guid, fileName);
		}

		static void AddToRegistry(const size_t mirrorType, const GUID& guid, const std::string& fileName);
#endif

	private:
		static std::unordered_map<size_t, AssetsMap> m_MapOfAssetMaps;

		static std::unordered_map<size_t, AssetsMap> m_RegistryFilePathsMapStructure;

		// Registry
		using ListFontPairs = std::vector<std::pair<GUID, std::string>>;
		// constexpr u16 materialsListSize = 11;
		using ListMaterialPairs = std::vector<std::pair<GUID, std::array<std::string, 11>>>;
		using ListMeshPairs = std::vector<std::pair<GUID, std::string>>;
		using ListShaders = std::vector<std::pair<GUID, std::pair<std::string, std::string>>>;
		using ListTextures = std::vector<std::pair<GUID, std::string>>;

		static ListFontPairs m_Fonts;
		static ListMaterialPairs m_Materials;
		static ListMeshPairs m_Meshes;
		static ListShaders m_Shaders;
		static ListTextures m_Textures;
	};

}
