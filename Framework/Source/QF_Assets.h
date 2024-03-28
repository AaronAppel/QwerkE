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

	QC_ENUM(eResourceType, int,
		Invalid = 0,
		Texture
	);

	namespace Assets {

		typedef int TextureHandle;
#define NullTextureHandle 0

		TextureHandle Load(const char* textureFilePath);
		TextureHandle Get(const char* textureName);

		bool Has(const char* textureName);

		template <FileExtensions extension>
		bool Get(const char* resourceName)
		{
			return extension == FileExtensions::fbx;
		}

	}

}
