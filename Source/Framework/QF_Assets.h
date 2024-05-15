#pragma once

#include <vector>

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

#include "QC_Guid.h"

enum class MirrorTypes;

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
			const MirrorTypes typeEnum = Mirror::InfoForType<T>()->enumType;
			std::unordered_map<GUID, T*>* assetMap = (std::unordered_map<GUID, T*>*)&m_MapOfAssetMaps[typeEnum];

			return assetMap->find(guid) != assetMap->end();
		}

		template <typename T>
		static T* Get(GUID guid)
		{
			const MirrorTypes typeEnum = Mirror::InfoForType<T>()->enumType;
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
			const MirrorTypes typeEnum = Mirror::InfoForType<T>()->enumType;
			std::unordered_map<GUID, T*>* assetMap = (std::unordered_map<GUID, T*>*)&m_MapOfAssetMaps[typeEnum];
			return *assetMap;
		}
		static std::unordered_map<MirrorTypes, AssetsList>& ViewRegistry();
		static void SaveRegistry();
		static void ExistsInRegistry(const GUID& guid, const std::string& fileName);
		static void AddToRegistry(const MirrorTypes mirrorType, const GUID& guid, const std::string& fileName);
#endif

	private:
		static std::unordered_map<MirrorTypes, AssetsMap> m_MapOfAssetMaps;
	};

}
