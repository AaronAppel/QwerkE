#pragma once

#include "QC_Guid.h"

namespace QwerkE {

	template <typename T>
	class AssetHandle
	{
	public:
		AssetHandle(T* asset, const GUID& guid) : m_Asset(asset), m_AssetGui(guid) { }

		operator bool() const { return m_Asset != nullptr; }

	private:
		T* m_Asset = nullptr;
		GUID m_AssetGui;
	};

}
