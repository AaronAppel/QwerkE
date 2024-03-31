#include "QF_Assets.h"

#include <string>
#include <unordered_map>

#include "QF_Files.h"

namespace QwerkE {

	namespace Assets {

		Texture s_NullTexture;
		static std::unordered_map<std::string, Texture*> s_Textures;

		bool local_Has(const char* textureName);
		const Texture& local_Load(const char* textureFilePath);

		const Texture& GetTexture(const char* assetName)
		{
			// Get extension
			// For a given extension, check if file is loaded
			// For a given extension, load if needed
			// For a loaded asset, return asset
			// Handle invalid (unloaded) asset case

			ASSERT(assetName, "Invalid assetName!");

			if (local_Has(assetName))
			{
				return *s_Textures[assetName];
			}
#ifdef _QDEBUG
			return local_Load(assetName); // #TODO Try to load asset
#else
			return AssetHandle(); // #TODO Resolve missing asset (null object?)
#endif
		}

		bool local_Has(const char* textureName)
		{
			ASSERT(textureName, "Invalid textureName!");
			return s_Textures.find(textureName) != s_Textures.end();
		}

		const Texture& local_Load(const char* textureFilePath)
		{
			ASSERT(textureFilePath, "Invalid textureFilePath!");

			Path fileName = Files::FileName(textureFilePath);
			// #TODO Make fileName.string().c_str() nicer to use
			if (local_Has(fileName.string().c_str()))
			{
#ifdef _QDEBUG
				LOG_WARN("{0} Asset already exists: {1}", __FUNCTION__, fileName.string().c_str());
#endif
				return *s_Textures[textureFilePath];
			}
			// #TODO Load asset
			return s_NullTexture;
		}

	}

}
