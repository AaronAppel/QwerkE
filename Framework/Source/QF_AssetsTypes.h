#pragma once

#ifdef _QENUM
#include "Libraries/enum/QC_enum.h"
#endif

namespace QwerkE {

	enum FileExtensions
	{
		invalid = 0,

		png,
		jpg,

		fbx,
		obj,
	};

	QC_ENUM(eAssetType, u8,
		// Invalid, Use AssetHandle::INVALID_HANDLE instead
		Texture
	);

	class AssetHandle
	{
	public:
		static const u8 INVALID_HANDLE = 0;

		const u8 HandleValue() { return m_HandleValue; }

		virtual eAssetType GetStaticType() = 0;

	protected:
		AssetHandle() = default;

	private:
		u8 m_HandleValue = INVALID_HANDLE;
	};

	class Texture
	{
	private:
		// DataBuffer m_Buffer;
	};

	class TextureHandle : public AssetHandle
	{
	public:
		TextureHandle(Texture* texture) : m_TexturePtr(texture) { }

		const Texture* const GetTexture() const { m_TexturePtr; }

		eAssetType GetStaticType() override { return eAssetType::Texture; }

	private:
		const Texture* const m_TexturePtr = nullptr;
	};

}
