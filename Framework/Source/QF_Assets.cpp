#include "QF_Assets.h"

#include <string>
#include <unordered_map>

#include "QF_FileSystem.h"
#include "QF_FileUtilities.h"

namespace QwerkE {

	namespace Assets {

		std::unordered_map<std::string, TextureHandle> s_Textures;
		// s_Materials
		// s_Shaders
		// s_Meshes
		// s_Sounds
		// s_Fonts

		TextureHandle Load(const char* textureFilePath)
		{
			ASSERT(textureFilePath, "Invalid textureFilePath!");

			uPtr<char> fileName = SmartFileName(textureFilePath, true);
			if (Has(fileName.get()))
			{
#ifdef _QDEBUG
				LOG_WARN("{0} Asset already exists: {1}", __FUNCTION__, fileName.get());
#endif
				return s_Textures[textureFilePath];
			}
			// #TODO Load asset
			return NullTextureHandle;
		}

		TextureHandle Get(const char* textureName)
		{
			ASSERT(textureName, "Invalid textureName!");

			if (Has(textureName))
			{
				return s_Textures[textureName];
			}
#ifdef _QDEBUG
			return Load(textureName); // #TODO Try to load asset
#else
			return NullTextureHandle; // #TODO Resolve missing asset (null object?)
#endif
		}

		bool Has(const char* textureName)
		{
			ASSERT(textureName, "Invalid textureName!");
			return s_Textures.find(textureName) != s_Textures.end();
		}

	}

}
