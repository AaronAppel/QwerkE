#pragma once

#include <array>
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
	using AssetsList = std::vector<std::pair<GUID, std::vector<std::string>>>;

	class Assets
	{
	public:
		Assets() = delete;

		static void Initialize();
		static void Shutdown();

		template <typename T>
		static bool Has(GUID guid)
		{
			const size_t typeId = Mirror::TypeId<T>();
			std::unordered_map<GUID, T*>* assetMap = (std::unordered_map<GUID, T*>*)&m_MapOfLoadedAssetMaps[typeId];

			return assetMap->find(guid) != assetMap->end();
		}

		template <typename T>
		static T* Get(GUID guid)
		{
			const size_t typeEnum = Mirror::TypeId<T>();
			std::unordered_map<GUID, T*>* assetMap = (std::unordered_map<GUID, T*>*)&m_MapOfLoadedAssetMaps[typeEnum];

			if (!Has<T>(guid))
			{
				guid = LoadAsset(Mirror::TypeId<T>(), guid);
			}

			if (m_MapOfLoadedAssetMaps.find(typeEnum) == m_MapOfLoadedAssetMaps.end() ||
				m_MapOfLoadedAssetMaps[typeEnum].find(guid) == m_MapOfLoadedAssetMaps[typeEnum].end())
			{
				ASSERT(Has<T>(GUID::Invalid), "No null asset found!");
			}

			void* assetPtr = m_MapOfLoadedAssetMaps[typeEnum][guid];
			return static_cast<T*>(assetPtr);
		}

#ifndef _QRETAIL
		template <typename T>
		static const std::unordered_map<GUID, T*>& ViewAssets()
		{
			const size_t typeEnum = Mirror::TypeId<T>();
			std::unordered_map<GUID, T*>* assetMap = (std::unordered_map<GUID, T*>*)&m_MapOfLoadedAssetMaps[typeEnum];
			return *assetMap;
		}

		static AssetsList& GetRegistryAssetList(const size_t assetListTypeId);

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
		static GUID LoadAsset(const size_t type, const GUID& guid);

		static std::unordered_map<size_t, AssetsMap> m_MapOfLoadedAssetMaps;
	};

}
