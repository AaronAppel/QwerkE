#include "QF_Assets.h"

#include <map>
#include <memory>

#ifdef GLEW
#include "Libraries/glew/glew.h"
#endif

#ifdef _QOPENAL
#include "Libraries/glew/glew.h"
#endif

#include "QF_Debug.h"
#include "QF_Defines.h"
#include "QF_FileSystem.h"
#include "QF_Jobs.h"
#include "QF_Material.h"
#include "QF_Mesh.h"
#include "QF_Serialization.h"
#include "QF_ShaderProgram.h"
#include "QF_Texture.h"
#include "QF_Settings.h"

namespace QwerkE {

	namespace Assets {

		static std::map<std::string, std::shared_ptr<Mesh*>> s_MeshesSmartPointers; // #TODO Try using smart pointers

		// #TODO Consider using unordered maps or linked lists. Review data structure choice
		static std::map<std::string, Mesh*> s_Meshes;
		static std::map<std::string, Texture*> s_Textures;
		static std::map<std::string, Material*> s_Materials;
		static std::map<std::string, FT_Face> s_Fonts; // #TODO Abstract freetype2
		static std::map<std::string, ALuint> s_Sounds; // #TODO Abstract OpenAL
		static std::map<std::string, ShaderProgram*> s_ShaderPrograms;
		static std::map<std::string, ShaderComponent*> s_ShaderComponents;

		Mesh* InstantiateFileMesh(const char* meshFilePath, const char* meshFileName);
		void InstantiateFileMeshes(const char* meshFilePath);
		Texture* InstantiateTextureAsync(const char* textureName);
		Texture* InstantiateTextureSync(const char* textureName);
		Material* InstantiateMaterial(const char* matName);
		FT_Face InstantiateFont(const char* fontName);
		ALuint InstantiateSound(const char* soundPath);
		ShaderProgram* InstantiateShaderProgram(const char* schematicFile);
		ShaderComponent* InstantiateShaderComponent(const char* componentFilePath);

		bool MeshLoaded(const char* filePath, Mesh* mesh);

		void Initialize()
		{
			ASSERT(s_Meshes.empty() && s_Textures.empty() && s_Materials.empty() && s_Fonts.empty() &&
				s_Sounds.empty() && s_ShaderPrograms.empty() && s_ShaderComponents.empty(), "Resources already initialized!");
		}

		void DeleteAllResources()
		{
			for (auto object : s_Meshes)
			{
				if (object.second)
				{
					delete object.second;
				}
			}

			for (auto object : s_Textures)
			{
				if (object.second)
				{
					glDeleteTextures(1, &object.second->s_Handle);
					delete object.second;
				}
			}

			for (auto object : s_Materials)
			{
				if (object.second)
				{
					delete object.second;
				}
			}

			s_Meshes.clear();
			s_Textures.clear();
			s_Materials.clear();
			s_Fonts.clear();
		}

		bool MeshExists(const char* name)
		{
			return name && s_Meshes.find(name) != s_Meshes.end();
		}

		bool TextureExists(const char* name)
		{
			return name && s_Textures.find(name) != s_Textures.end();
		}

		bool MaterialExists(const char* name)
		{
			return name && s_Materials.find(name) != s_Materials.end();
		}

		bool FontExists(const char* name)
		{
			return name && s_Fonts.find(name) != s_Fonts.end();
		}

		bool SoundExists(const char* name)
		{
			return name && s_Sounds.find(name) != s_Sounds.end();
		}

		bool ShaderProgramExists(const char* name)
		{
			return name && s_ShaderPrograms.find(name) != s_ShaderPrograms.end();
		}

		bool ShaderComponentExists(const char* name)
		{
			return name && s_ShaderComponents.find(name) != s_ShaderComponents.end();
		}

		static unsigned int g_UniqueID = 0;
		int GetGUID() // #FEATURE F0004 - GUIDs
		{
			return g_UniqueID++;
		}

		// TODO: Handle errors and deleting assets before returning nullptr
		Mesh* InstantiateFileMesh(const char* meshFilePath, const char* meshFileName)
		{
			if (!meshFilePath || !meshFileName)
			{
				LOG_ERROR("{0} Null mesh file path or mesh name!", __FUNCTION__);
				return s_Meshes[null_mesh];
			}

			if (FileExists(meshFilePath))
			{
				if (Mesh* mesh = Files::LoadMeshInModelByName(meshFilePath, meshFileName))
				{
					LOG_TRACE("{0} Mesh loaded {1}", __FUNCTION__, mesh->GetName().c_str());
					Assets::MeshLoaded(mesh->GetName().c_str(), mesh);
					return mesh;
				}
				else
				{
					LOG_ERROR("{0} Unable to load mesh {1} from file {2}", __FUNCTION__, meshFilePath, meshFileName);
				}
			}

			ASSERT(s_Meshes.size() > 0, "No valid asset to return!");
			return s_Meshes[null_mesh];
		}

		void InstantiateFileMeshes(const char* meshFilePath)
		{
			if (!meshFilePath)
			{
				LOG_ERROR("{0} Null mesh file path!", __FUNCTION__);
				return;
			}

			if (FileExists(meshFilePath))
			{
				std::vector<Mesh*> meshes = Files::LoadModelFileToMeshes(meshFilePath);

				for (size_t i = 0; i < meshes.size(); i++)
				{
					if (meshes[i])
					{
						MeshLoaded(meshes[i]->GetFileName().c_str(), meshes[i]);
						LOG_TRACE("{0} Mesh loaded {1}", __FUNCTION__, meshes[i]->GetName().c_str());
					}
					else
					{
						LOG_WARN("{0} Null mesh found in file {1}", __FUNCTION__, meshFilePath);
					}
				}
			}
		}

		Texture* InstantiateTextureSync(const char* absoluteTextureFilePath)
		{
			if (FileExists(absoluteTextureFilePath) && !TextureExists(SmartFileName(absoluteTextureFilePath, true).get()))
			{
				Texture* texture = new Texture();
				texture->s_Handle = Load2DTexture(absoluteTextureFilePath);
				if (uPtr<char> fullFileName = SmartFileName(absoluteTextureFilePath, true))
				{
					texture->s_FileName = fullFileName.get();
				}

				if (texture->s_Handle != 0)
				{
					s_Textures[texture->s_FileName] = texture;
					return texture;
				}

				delete texture;
				LOG_WARN("{0} Invalid texture handle: {1}", __FUNCTION__, absoluteTextureFilePath);
			}
			else
			{
				LOG_WARN("{0} Texture not found: {1}", __FUNCTION__, absoluteTextureFilePath);
			}

			ASSERT(s_Textures.size() > 0, "No valid asset to return!");
			return s_Textures[null_texture];
		}

		Texture* InstantiateTextureAsync(const char* absoluteTextureFilePath)
		{
			if (!absoluteTextureFilePath)
				return s_Textures[null_texture];

			uPtr<char> fullFileName = SmartFileName(absoluteTextureFilePath, true);
			if (FileExists(absoluteTextureFilePath) && !TextureExists(fullFileName.get()))
			{
				QLoadAsset* newLoadAssetJob = new QLoadAsset(absoluteTextureFilePath);
				Jobs::ScheduleJob(newLoadAssetJob);

				Texture* newTexture = new Texture();
				newTexture->s_Handle = s_Textures[null_texture]->s_Handle;
				s_Textures[fullFileName.get()] = newTexture;
				return newTexture;
			}
			else
			{
				LOG_WARN("{0} Texture not found: {1}", __FUNCTION__, absoluteTextureFilePath);
			}

			ASSERT(s_Textures.size() > 0, "No valid asset to return!");
			return s_Textures[null_texture];
		}

		Material* InstantiateMaterial(const char* absoluteMaterialFilePath)
		{
			if (!absoluteMaterialFilePath)
				return nullptr;

			Material* material = nullptr;

			if (strcmp(SmartFileExtension(absoluteMaterialFilePath).get(), material_schematic_ext) == 0)
			{
				if (FileExists(absoluteMaterialFilePath))
				{
					material = new Material();
					Serialization::DeserializeJsonFromFile(absoluteMaterialFilePath, *material);
				}
				else
				{
					LOG_ERROR("{0} Could not find .msch file named: {1}!", __FUNCTION__, absoluteMaterialFilePath);
				}
			}
			else
			{
				LOG_WARN("{0} Not a {1} file: {2}", __FUNCTION__, shader_schematic_ext, absoluteMaterialFilePath);
			}

			if (!material)
			{
				LOG_WARN("{0} Material not found: {1}", __FUNCTION__, absoluteMaterialFilePath);
				ASSERT(s_Materials.size() > 0, "No valid asset to return!");
				return s_Materials[null_material];
			}

			if (uPtr<char> fullFileName = SmartFileName(absoluteMaterialFilePath, true))
			{
				s_Materials[fullFileName.get()] = material;
			}
			return material;
		}

		FT_Face InstantiateFont(const char* fontName)
		{
			FT_Face font;
			FT_Library ft = NULL; // TODO: No need to reload ft library

			static bool triedInit = false; // TODO: Improve logic
			if (triedInit == false)
			{
				triedInit = true;

				if (FT_Init_FreeType(&ft))
					LOG_ERROR("ERROR::FREETYPE: Could not init FreeType Library");
			}

			if (FT_New_Face(ft, fontName, 0, &font))
			{
				LOG_ERROR("ERROR::FREETYPE: Failed to load font");
				ASSERT(s_Fonts.size() > 0, "No valid asset to return!");
				return s_Fonts[null_font];
			}
			s_Fonts[fontName] = font;
			FT_Done_FreeType(ft);
			assert(font != NULL);
			return font;
		}

		ALuint InstantiateSound(const char* soundPath)
		{
			ALuint handle = 0;
			handle = Files::LoadSound(soundPath);

			if (handle != 0)
			{
				if (uPtr<char> fullFileName = SmartFileName(soundPath, true))
				{
					s_Sounds[fullFileName.get()] = handle;
				}
				return handle;
			}
			ASSERT(s_Sounds.size() > 0, "No valid asset to return!");
			return s_Sounds[null_sound];
		}

		ShaderProgram* InstantiateShaderProgram(const char* schematicFile)
		{
			if (FileExists(schematicFile))
			{
				ShaderProgram* newShaderProgram = new ShaderProgram();
				Serialization::DeserializeJsonFromFile(schematicFile, *newShaderProgram);

				newShaderProgram->SetVertShader(GetShaderComponent(newShaderProgram->GetVertName().c_str(), eShaderComponentTypes::Vertex));
				newShaderProgram->SetFragShader(GetShaderComponent(newShaderProgram->GetFragName().c_str(), eShaderComponentTypes::Fragment));
				// newShaderProgram->s_geoShader = GetShaderComponentData(newShaderProgram->s_geoName.c_str(), eShaderComponentTypes::Geometry);

				if (ShaderFactory::LinkCreatedShaderProgram(newShaderProgram))
				{
					if (uPtr<char> fullFileName = SmartFileName(schematicFile, true))
					{
						s_ShaderPrograms[fullFileName.get()] = newShaderProgram;
					}
					newShaderProgram->FindAttributesAndUniforms();
					return newShaderProgram;
				}
				delete newShaderProgram;
			}
			ASSERT(s_ShaderPrograms.size() > 0, "No valid asset to return!");
			return s_ShaderPrograms[null_shader_schematic];
		}

		ShaderComponent* InstantiateShaderComponent(const char* componentFilePath)
		{
			// ASSERT(componentFilePath && FileExists(componentFilePath), "Cannot return a valid shader component using an invalid file path!");

			uPtr<char> fullFileName = SmartFileName(componentFilePath, true);
			uPtr<char> fileExtension = SmartFileExtension(componentFilePath);

			if (strcmp(fileExtension.get(), vertex_shader_ext) == 0)
			{
				if (ShaderComponent* result = ShaderFactory::CreateShaderComponent(GL_VERTEX_SHADER, componentFilePath))
				{
					s_ShaderComponents[fullFileName.get()] = result;
					return result;
				}
				ASSERT(s_ShaderComponents.size() > 0, "No valid asset to return!");
				return s_ShaderComponents[null_vert_component];
			}
			else if (strcmp(fileExtension.get(), fragment_shader_ext) == 0)
			{
				if (ShaderComponent* result = ShaderFactory::CreateShaderComponent(GL_FRAGMENT_SHADER, componentFilePath))
				{
					s_ShaderComponents[fullFileName.get()] = result;
					return result;
				}
				ASSERT(s_ShaderComponents.size() > 0, "No valid asset to return!");
				return s_ShaderComponents[null_frag_component];
			}
			else if (strcmp(fileExtension.get(), geometry_shader_ext) == 0)
			{
				LOG_WARN("Geometry shaders are not yet supported");
				if (ShaderComponent* result = ShaderFactory::CreateShaderComponent(GL_GEOMETRY_SHADER, componentFilePath))
				{
					s_ShaderComponents[fullFileName.get()] = result;
					return result;
				}
				ASSERT(s_ShaderComponents.size() > 0, "No valid asset to return!");
				return s_ShaderComponents[null_geo_component];
			}
			ASSERT(false, "Invalid shader component type from file extension");
			return nullptr;
		}

		Mesh* GetMesh(const char* meshName)
		{
			if (meshName && MeshExists(meshName))
				return s_Meshes[meshName];

			ASSERT(s_Meshes.size() > 0, "No valid asset to return!");
			return s_Meshes[null_mesh];
		}

		Mesh* GetMeshFromFile(const char* absoluteFilePath, const char* meshName)
		{
			// #TODO Validate meshName as a proper name
			if (!meshName)
				return s_Meshes[null_mesh];

			if (MeshExists(meshName))
				return s_Meshes[meshName];

			if (Mesh* result = Files::LoadMeshInModelByName(absoluteFilePath, meshName))
			{
				s_Meshes[meshName] = result;
				return result;
			}

			ASSERT(s_Meshes.size() > 0, "No valid asset to return!");
			return s_Meshes[null_mesh];
		}

		Texture* GetTexture(const char* textureName)
		{
			if (!textureName)
				return s_Textures[null_texture];

			if (TextureExists(textureName))
				return s_Textures[textureName];

			if (uPtr<char > fullFileName = SmartFileName(textureName, true))
			{
				if (TextureExists(fullFileName.get()))
					return s_Textures[fullFileName.get()];
			}

			ASSERT(s_Textures.size() > 0, "No valid asset to return!");
			return s_Textures[null_texture];
		}

		Texture* GetTextureFromPath(const char* filePath, const bool async)
		{
			if (!filePath)
			{
				ASSERT(s_Textures.size() > 0, "No valid asset to return!");
				return s_Textures[null_texture];
			}

			if (uPtr<char > fullFileName = SmartFileName(filePath, true))
			{
				if (TextureExists(fullFileName.get()))
					return s_Textures[fullFileName.get()];
			}

			if (async)
			{
				return InstantiateTextureAsync(filePath);
			}
			return InstantiateTextureSync(filePath);
		}

		Material* GetMaterial(const char* name)
		{
			if (!name)
				return s_Materials[null_material];

			if (s_Materials.find(name) != s_Materials.end())
				return s_Materials[name];

			return InstantiateMaterial(SchematicsFolderPath(name));
		}

		Material* GetMaterialFromPath(const char* filePath)
		{
			if (!filePath)
				return s_Materials[null_material];

			if (uPtr<char> fullFileName = SmartFileName(filePath, true))
			{
				if (MaterialExists(fullFileName.get()))
					return s_Materials[fullFileName.get()];
			}

			return InstantiateMaterial(filePath);
		}

		FT_Face GetFont(const char* name)
		{
			if (!name)
				return s_Fonts[null_font];

			if (s_Fonts.find(name) != s_Fonts.end())
				return s_Fonts[name];

			return InstantiateFont(FontsFolderPath(name));
		}

		FT_Face GetFontFromPath(const char* filePath)
		{
			if (!filePath)
				return s_Fonts[null_font];

			if (uPtr<char> fullFileName = SmartFileName(filePath, true))
			{
				if (FontExists(fullFileName.get()))
					return s_Fonts[fullFileName.get()];
			}

			return InstantiateFont(filePath);
		}

		ALuint GetSound(const char* name)
		{
			if (!name)
				return s_Sounds[null_sound];

			if (SoundExists(name))
				return s_Sounds[name];

			return InstantiateSound(SoundsFolderPath(name));
		}

		ALuint GetSoundFromPath(const char* filePath)
		{
			if (!filePath)
				return s_Sounds[null_sound];

			if (uPtr<char> fullFileName = SmartFileName(filePath, true))
			{
				if (SoundExists(fullFileName.get()))
					return s_Sounds[fullFileName.get()];
			}
			return InstantiateSound(filePath);
		}

		ShaderProgram* GetShaderProgram(const char* name)
		{
			if (!name)
				return s_ShaderPrograms[null_shader];

			if (ShaderProgramExists(name))
				return s_ShaderPrograms[name];

			return InstantiateShaderProgram(SchematicsFolderPath(name));
		}

		ShaderProgram* GetShaderProgramFromPath(const char* filePath)
		{
			if (!filePath)
				return s_ShaderPrograms[null_shader];

			if (uPtr<char> fullFileName = SmartFileName(filePath, true))
			{
				if (ShaderProgramExists(fullFileName.get()))
					return s_ShaderPrograms[fullFileName.get()];

				return InstantiateShaderProgram(filePath);
			}
			ASSERT(s_ShaderPrograms.size() > 0, "No valid asset to return!");
			return s_ShaderPrograms[null_shader];
		}

		ShaderComponent* GetShaderComponent(const char* name, eShaderComponentTypes componentType)
		{
			if (!name)
			{
				switch (componentType)
				{
				case QwerkE::Vertex:
					return s_ShaderComponents[StringAppend(null_shader, vertex_shader_ext)];
					break;

				case QwerkE::Fragment:
					return s_ShaderComponents[StringAppend(null_shader, fragment_shader_ext)];
					break;

				case QwerkE::Geometry:
					return s_ShaderComponents[StringAppend(null_shader, geometry_shader_ext)];
					break;

				default:
					ASSERT(false, "Cannot determine return shader component for null filePath!");
					break;
				}
			}

			if (ShaderComponentExists(name))
				return s_ShaderComponents[name];

			return InstantiateShaderComponent(ShadersFolderPath(name));
		}

		ShaderComponent* GetShaderComponentFromPath(const char* filePath, eShaderComponentTypes componentType)
		{
			if (!filePath)
			{
				switch (componentType)
				{
				case QwerkE::Vertex:
					return s_ShaderComponents[StringAppend(null_shader, vertex_shader_ext)];
					break;

				case QwerkE::Fragment:
					return s_ShaderComponents[StringAppend(null_shader, fragment_shader_ext)];
					break;

				case QwerkE::Geometry:
					return s_ShaderComponents[StringAppend(null_shader, geometry_shader_ext)];
					break;

				default:
					ASSERT(false, "Cannot determine return shader component for null filePath!");
					break;
				}
			}

			if (uPtr<char> fullFileName = SmartFileName(filePath, true))
			{
				if (ShaderComponentExists(fullFileName.get()))
				{
					return s_ShaderComponents[fullFileName.get()];
				}
			}

			return InstantiateShaderComponent(filePath);
		}

		const std::map<std::string, Mesh*>& SeeMeshes() { return s_Meshes; };
		const std::map<std::string, Texture*>& SeeTextures() { return s_Textures; };
		const std::map<std::string, Material*>& SeeMaterials() { return s_Materials; };
		const std::map<std::string, ALuint>& SeeSounds() { return s_Sounds; };
		const std::map<std::string, ShaderProgram*>& SeeShaderPrograms() { return s_ShaderPrograms; };
		const std::map<std::string, ShaderComponent*>& SeeShaderComponents() { return s_ShaderComponents; };

		void LoadMeshesFromFile(const char* absoluteMeshFilePath)
		{
			if (absoluteMeshFilePath)
			{
				InstantiateFileMeshes(absoluteMeshFilePath);
			}
		}

		Material* CreateNewMaterialFromFile(const char* absoluteMaterialFilePath)
		{
			if (!absoluteMaterialFilePath)
				return nullptr;

			Material* newMaterial = nullptr;
			if (strcmp(SmartFileExtension(absoluteMaterialFilePath).get(), material_schematic_ext) == 0)
			{
				if (FileExists(absoluteMaterialFilePath))
				{
					newMaterial = new Material();
					Serialization::DeserializeJsonFromFile(absoluteMaterialFilePath, *newMaterial);
				}
				else
				{
					LOG_ERROR("{0} Could not find {1} file named: {2}!", __FUNCTION__, material_schematic_ext, absoluteMaterialFilePath);
				}
			}
			else
			{
				LOG_WARN("{0} Not a {1} file: {2}", __FUNCTION__, shader_schematic_ext, absoluteMaterialFilePath);
			}

			if (!newMaterial)
			{
				LOG_WARN("{0} Material not found: {1}", __FUNCTION__, absoluteMaterialFilePath);
				ASSERT(s_Materials.size() > 0, "No valid asset to return!");
				return s_Materials[null_material];
			}

			while (MaterialExists(newMaterial->GetMaterialName().c_str()) || FileExists(SchematicsFolderPath(absoluteMaterialFilePath)))
			{
				std::string uniqueName = NumberAppendOrIncrementFileName(newMaterial->GetMaterialName().c_str());
				uniqueName += ".";
				uniqueName += material_schematic_ext;
				newMaterial->SetMaterialName(uniqueName);
			}

			s_Materials[newMaterial->GetMaterialName().c_str()] = newMaterial;
			return newMaterial;
		}

		void LoadAsset(const char* assetFilePath)
		{
			uPtr<char> fullFileName = SmartFileExtension(assetFilePath);
			uPtr<char> fileExtension = SmartFileExtension(assetFilePath);

			if (strcmp(fileExtension.get(), ".png") == 0 ||
				strcmp(fileExtension.get(), ".jpg") == 0)
			{
				if (!TextureExists(fullFileName.get()))
				{
					Texture* newTexture = GetTextureFromPath(fullFileName.get(), true);
					s_Textures[fullFileName.get()] = newTexture;
				}
			}
			else if (strcmp(fileExtension.get(), ".obj") == 0)
			{
				if (!MeshExists(fullFileName.get()))
				{
					// std::vector<Mesh*> newMeshes = InstantiateFileMeshes(fullFileName.get());
					//
					// for (size_t i = 0; i < newMeshes.size(); i++)
					// {
					// 	if (newMeshes[i])
					// 	{
					// 		s_Meshes[newMeshes[i]->GetName().c_str()] = newMeshes[i];
					// 	}
					// }
				}
			}
		}

		void TextureLoaded(const char* absoluteTextureFilePath, int handle)
		{
			if (absoluteTextureFilePath) // #TODO Verify handle value?  && handle != 0
			{
				if (uPtr<char> fullFileName = SmartFileName(absoluteTextureFilePath, true))
				{
					if (TextureExists(fullFileName.get()))
					{
						s_Textures[fullFileName.get()]->s_Handle = (GLuint)handle;
					}
				}
			}
		}

		bool TextureLoaded(const char* name, Texture* texture)
		{
			if (!name || !texture || texture->s_Handle == 0)
				return false;

			if (TextureExists(name))
				return false;

			s_Textures[name] = texture;
			return true;
		}

		bool MeshLoaded(const char* filePath, Mesh* mesh)
		{
			if (!filePath || mesh == nullptr || mesh->GetName() == Constants::gc_DefaultStringValue)
				return false;

			const char* meshName = mesh->GetName().c_str();
			if (MeshExists(meshName))
				return false;

			s_Meshes[meshName] = mesh;
			return true;
		}

		bool MaterialLoaded(const char* name, Material* material)
		{
			// EarlyReturnIfNull(2, name, material);

			if (!name || !material || material->GetMaterialName() == Constants::gc_DefaultStringValue)
				return false;

			if (MaterialExists(name))
				return false;

			s_Materials[name] = material;
			return true;
		}

		bool FontLoaded(const char* name, FT_Face font)
		{
			if (!name || !font || FontExists(name))
				return false;

			// TODO: How to check for null font? Also need cross library font classes
			// if (font == nullptr || material->name == gc_DefaultStringValue)
				// return false;

			s_Fonts[name] = font;
			return true;
		}

		bool SoundLoaded(const char* name, ALuint sound)
		{
			if (!name || sound == 0)
				return false;

			if (SoundExists(name))
				return false;

			s_Sounds[name] = sound;
			return true;
		}

		bool ShaderProgramLoaded(ShaderProgram* shaderProgram)
		{
			if (!shaderProgram || !shaderProgram->GetName().c_str())
				return false;

			return ShaderProgramLoaded(shaderProgram->GetName().c_str(), shaderProgram);
		}

		bool ShaderProgramLoaded(const char* name, ShaderProgram* ShaderProgram)
		{
			if (!name || !ShaderProgram)
				return false;

			if (ShaderProgramExists(name))
				return false;

			s_ShaderPrograms[name] = ShaderProgram;
			return true;
		}

		bool ShaderComponentLoaded(const char* name, ShaderComponent* shaderComponent)
		{
			if (!name || shaderComponent == nullptr)
				return false;

			if (ShaderComponentExists(name))
				return false;

			s_ShaderComponents[name] = shaderComponent;
			return true;
		}

	}

}
