#pragma once

#include <array>
#include <vector>

#ifdef _QBGFX
#include "Libraries/bgfx/include/bgfx/bgfx.h"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Structs.h"
#endif

#include "QC_Guid.h"

namespace QwerkE {

	using AssetsMap = std::unordered_map<GUID, void*>; // #NOTE Smart pointers require compile time types. Could look at another solution
	using AssetsList = std::vector<std::pair<GUID, std::vector<std::string>>>; // #TODO should just be string, not vector of string. Change once shader components are in

	class Assets
	{
	public:
		Assets() = delete;

		static void Initialize();
		static void Shutdown();

		template <typename T>
		static bool Has(GUID guid)
		{
			const size_t typeId = Mirror::IdForType<T>();
			if (m_MapOfLoadedAssetMaps.find(typeId) == m_MapOfLoadedAssetMaps.end())
				return false;

			std::unordered_map<GUID, T*>* assetMap = (std::unordered_map<GUID, T*>*)&m_MapOfLoadedAssetMaps[typeId];
			return assetMap->find(guid) != assetMap->end();
		}

		template <typename T>
		[[nodiscard]] static T* Get(GUID guid)
		{
			Load<T>(guid);

			const size_t typeId = Mirror::IdForType<T>();
			if (m_MapOfLoadedAssetMaps.find(typeId) == m_MapOfLoadedAssetMaps.end() ||
				m_MapOfLoadedAssetMaps[typeId].find(guid) == m_MapOfLoadedAssetMaps[typeId].end())
			{
				ASSERT(Has<T>(GUID::Invalid), "No null asset found!");
			}

			void* assetPtr = m_MapOfLoadedAssetMaps[typeId][guid];
			return static_cast<T*>(assetPtr);
		}

		template <typename T>
		static void Load(GUID guid)
		{
			const size_t typeId = Mirror::IdForType<T>();

			if (!Has<T>(guid))
			{
				guid = LoadAsset(typeId, guid);
			}
		}

// #TODO Decide how Release and Retail builds should work, in Game and Editor projects
// #ifndef _QRETAIL
		template <typename T>
		static const std::unordered_map<GUID, T*>* ViewAssets()
		{
			const size_t typeId = Mirror::IdForType<T>();
			std::unordered_map<GUID, T*>* assetMap = nullptr;
			if (m_MapOfLoadedAssetMaps.find(typeId) != m_MapOfLoadedAssetMaps.end())
			{
				// #TODO Avoid below C-style cast
				// assetMap = static_cast<std::unordered_map<GUID, T*>*>(&m_MapOfLoadedAssetMaps[typeId]);
				assetMap = (std::unordered_map<GUID, T*>*) & m_MapOfLoadedAssetMaps[typeId];
			}
			return assetMap;
		}

		static AssetsList& GetRegistryAssetList(const size_t assetListTypeId);

		template <typename T>
		static std::string GetRegistryAssetFileName(const GUID guid)
		{
			const size_t typeId = Mirror::IdForType<T>();
			const AssetsList& assetsRegistry = Assets::GetRegistryAssetList(typeId);
			for (size_t i = 0; i < assetsRegistry.size(); i++)
			{
				auto guidStringPair = assetsRegistry[i];
				if (guid == guidStringPair.first)
				{
					// #TODO Decide how to search for shader and materials that have more than 1 string in vector
					return guidStringPair.second[0];
				}
			}
			return std::string("");
		}

		static std::unordered_map<size_t, AssetsList>& ViewRegistry();
		static void SaveRegistry();

		static bool Assets::ExistsInRegistry(const size_t mirrorTypeId, const GUID& guid, const std::string& fileName);

		// #TODO Unable to use currently. Compiler error on argument conversion
		template <typename T>
		static bool ExistsInRegistry(const GUID& guid, const std::string& fileName)
		{
			return ExistsInRegistry(Mirror::IdForType<T>, guid, fileName);
		}

		static void AddToRegistry(const size_t mirrorType, const GUID& guid, const std::string& fileName);
// #endif // _QRETAIL

	private:
		static GUID LoadAsset(const size_t type, const GUID& guid);

		static std::unordered_map<size_t, AssetsMap> m_MapOfLoadedAssetMaps;
	};

}
