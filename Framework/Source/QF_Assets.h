#pragma once

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

#include "QC_Guid.h"

namespace QwerkE {

	class Mesh;
	class Shader;

	using AssetsMap = std::unordered_map<GUID, void*>;

	class Assets
	{
	public:
		Assets() = delete;
		~Assets() = delete;

		static const bgfx::ProgramHandle& GetProgram() { return m_ProgramCube; }

		template <typename T>
		static bool Has(const std::unordered_map<GUID, sPtr<T>>& assetMap, const GUID& guid)
		{
			// ASSERT(guid != GUID::Invalid, "Invalid guid!");
			return assetMap.find(guid) != assetMap.end();
		}

		// const sPtr<T> local_Load(const std::unordered_map<GUID, sPtr<T>>& assetMap, const GUID& guid);

		template <typename T>
		static sPtr<T> Get(GUID guid)
		{
			const MirrorTypes typeEnum = Mirror::InfoForType<T>()->enumType;
			std::unordered_map<GUID, sPtr<T>>* assetMap = (std::unordered_map<GUID, sPtr<T>>*)&m_MapOfAssetMaps[typeEnum];

			if (!Has(*assetMap, guid))
			{
				guid = GUID::Invalid;
			}

			void* assetPtr = m_MapOfAssetMaps[typeEnum][guid];
			return *(sPtr<T>*)assetPtr;
		}

		static void Initialize();
		static void Shutdown();

#ifndef _QRETAIL
		// For editor only, really. Maybe find another use for exposing assets collections
		// map<TypeId, map<Id, Asset>> ViewAssets();
#endif

	private:
		static sPtr<Mesh> m_NullMesh;
		static sPtr<Shader> m_NullShader;

		static std::unordered_map<GUID, sPtr<Mesh>> m_Meshes;
		static std::unordered_map<MirrorTypes, AssetsMap> m_MapOfAssetMaps;

		static bgfx::ProgramHandle m_ProgramCube;
	};

}
