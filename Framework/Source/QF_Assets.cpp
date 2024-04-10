#include "QF_Assets.h"

#include <string>
#include <unordered_map>

#ifdef _QBGFXFRAMEWORK
#include <bgfxFramework/SampleRenderData.h>
#include <bgfxFramework/LoadShader.h>
#endif

#include "QF_Files.h"

namespace QwerkE {

	namespace Assets {

		Texture s_NullTexture;
		static std::unordered_map<std::string, Texture*> s_Textures;

		Texture s_NullMesh;
		static std::unordered_map<std::string, Texture*> s_Meshes;

		bool local_Has(const char* textureName);
		const Texture& local_Load(const char* textureFilePath);

		static bgfx::VertexBufferHandle s_vbhCube;
		static bgfx::IndexBufferHandle s_ibhCube;
		static bgfx::ProgramHandle s_programCube;

		void Initialize()
		{
			s_vbhCube = bgfx::createVertexBuffer(
				// Static data can be passed with bgfx::makeRef
				bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices))
				, PosColorVertex::ms_layout
			);

			s_ibhCube = bgfx::createIndexBuffer(
				// Static data can be passed with bgfx::makeRef
				bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
			);

			// Create program from shaders.
			s_programCube = myLoadShaderProgram("vs_cubes.bin", "fs_cubes.bin");
		}

		void Shutdown()
		{
			bgfx::destroy(s_vbhCube);
			bgfx::destroy(s_ibhCube);
			bgfx::destroy(s_programCube);
		}

		const bgfx::VertexBufferHandle& GetVbh()
		{
			return s_vbhCube;
		}

		const bgfx::IndexBufferHandle& GetIbh()
		{
			return s_ibhCube;
		}

		const bgfx::ProgramHandle& GetProgram()
		{
			return s_programCube;
		}

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
