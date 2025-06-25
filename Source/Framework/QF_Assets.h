#pragma once

#include <array>
#include <string>
#include <vector>

#ifdef _QBGFX
#include "Libraries/bgfx/include/bgfx/bgfx.h"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Structs.h"
#endif

#include "QC_Guid.h"

struct GLFWwindow;

namespace QwerkE {

	using AssetsMap = std::unordered_map<GUID, void*>; // #NOTE Smart pointers require compile time types. Could look at another solution
	using AssetsList = std::vector<std::pair<GUID, std::vector<std::string>>>; // #TODO should just be string, not vector of string. Change once shader components are in

	class Scene;
	namespace Scenes
	{
		Scene* CreateScene(const GUID& sceneGuid);
		Scene* CreateScene(const std::string& a_SceneFileName);
	}

	namespace Window
	{
		void local_FileDropCallback(GLFWwindow* window, int fileCount, const char** filePaths);
	}

	class Assets // #NOTE Class to hide static members that are used in public templated methods, and for friend functions
	{
	public:
		Assets() = delete;

		static void Initialize();
		static void Shutdown();

		template <typename T>
		[[nodiscard]] static void* Create(std::string a_FileName) // #TODO bool a_AddToRegistry?
		{
			ASSERT(false, "Not implemented!")

			// #TODO See if restricting Assets to manage all asset allocations is safe but still convenient enough.
			// See if there is a generic API for creating all asset types.
			// See if LoadAsset can be updated to register new asset instances and registry entries.
			// if (!Has<T>(a_FileName)) // #TODO May need a_fileName check to see if the instance already exists
			{
				const size_t typeId = Mirror::IdForType<T>();
				switch (typeId)
				{
				case Mirror::IdForType<Scene>():
					Scene* newScene = CreateScene(const std::string & a_SceneFileName)
					// #TODO Add to registry?
					AddToRegistry(typeId, newScene->GetGuid(), a_SceneFileName);
					return newScene;

				default:
					// #TODO Error, unhandled type!
					break;
				}
			}
			return nullptr;
		}

		template <typename T>
		static void Load(GUID guid)
		{
			if (!Has<T>(guid))
			{
				const size_t typeId = Mirror::IdForType<T>();
				if (GUID::Invalid == LoadAsset(typeId, guid))
				{
					LOG_WARN("Unable to load {0} asset with GUID: {1}", Mirror::InfoForType<T>()->stringName.c_str(), guid);
				}
			}
		}

		template <typename T>
		static void Add(T* a_Instance, GUID a_Guid) // #TODO Meant for editor only creation of new asset files
		{
			if (!Has<T>(a_Guid) && GUID::Invalid != a_Guid)
			{
				const size_t typeId = Mirror::IdForType<T>();
				m_MapOfLoadedAssetMaps[typeId][a_Guid] = a_Instance;
				// #TODO Add to registry?
				// AddToRegistry(Mirror::IdForType<T>(), guid, a_SceneFileName);
			}
		}

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
				LOG_WARN("No loaded {0} with GUID: {1}", Mirror::InfoForType<T>()->stringName.c_str(), guid);
				ASSERT(m_MapOfNullAssetMaps[typeId].find(GUID::Invalid) != m_MapOfNullAssetMaps[typeId].end(), "No null asset found!");
				void* assetPtr = m_MapOfNullAssetMaps[typeId][GUID::Invalid];
				return static_cast<T*>(assetPtr);
			}

			void* assetPtr = m_MapOfLoadedAssetMaps[typeId][guid];
			return static_cast<T*>(assetPtr);
		}

// #TODO Decide how Release and Retail builds should work, in Game and Editor projects
// #ifndef _QRETAIL
		template <typename T>
		[[nodiscard]] static const std::unordered_map<GUID, T*>* ViewAssets()
		{
			const size_t typeId = Mirror::IdForType<T>();
			const std::unordered_map<GUID, T*>* assetMap = nullptr;
			if (m_MapOfLoadedAssetMaps.find(typeId) != m_MapOfLoadedAssetMaps.end())
			{
				// #TODO Avoid C-style cast
				// assetMap = static_cast<const std::unordered_map<GUID, T*>*>(&m_MapOfLoadedAssetMaps[typeId]);
				assetMap = (const std::unordered_map<GUID, T*>*) &m_MapOfLoadedAssetMaps[typeId];
			}
			return assetMap;
		}

		template <typename T>
		[[nodiscard]] static std::string GetRegistryAssetFileName(const GUID guid)
		{
			const size_t typeId = Mirror::IdForType<T>();
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

		[[nodiscard]] static std::unordered_map<size_t, AssetsList>& ViewRegistry();
		static void SaveRegistry();

		[[nodiscard]] static bool Assets::ExistsInRegistry(const size_t mirrorTypeId, const GUID& guid, const std::string& fileName);

		// #TODO Unable to use currently. Compiler error on argument conversion
		template <typename T>
		[[nodiscard]] static bool ExistsInRegistry(const GUID& guid, const std::string& fileName)
		{
			return ExistsInRegistry(Mirror::IdForType<T>, guid, fileName);
		}

// #endif // _QRETAIL

	private:
		friend void Window::local_FileDropCallback(GLFWwindow* window, int fileCount, const char** filePaths);
		friend Scene* Scenes::CreateScene(const std::string& a_SceneFileName);
		friend Scene* Scenes::CreateScene(const GUID& sceneGuid);

		static void AddToRegistry(const size_t mirrorType, const GUID& guid, const std::string& fileName);

		static GUID LoadAsset(const size_t type, const GUID& guid);
		static AssetsList& GetRegistryAssetList(const size_t assetListTypeId);

		static std::unordered_map<size_t, AssetsMap> m_MapOfLoadedAssetMaps;
		static std::unordered_map<size_t, AssetsMap> m_MapOfNullAssetMaps; // Safe default assets to use in cases of errors. Only the framework should care about these
	};

}
