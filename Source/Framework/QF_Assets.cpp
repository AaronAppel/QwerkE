#include "QF_Assets.h"

#include <map>
#include <memory>

#include "Libraries/glew/GL/glew.h"

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

		Mesh* InstantiateMesh(const char* meshFilePath);
		Texture* InstantiateTexture(const char* textureName);
		Material* InstantiateMaterial(const char* matName);
		FT_Face InstantiateFont(const char* fontName);
		ALuint InstantiateSound(const char* soundPath);
		ShaderProgram* InstantiateShaderProgram(const char* schematicFile);
		ShaderComponent* InstantiateShaderComponent(const char* componentFilePath);

		void Initialize()
		{
			ASSERT(s_Meshes.empty() && s_Textures.empty() &&
				s_Materials.empty() && s_Fonts.empty() &&
				s_Sounds.empty() && s_ShaderPrograms.empty() &&
				s_ShaderComponents.empty(),
				"Resources already initialized!");

			ASSERT(InstantiateMesh(MeshesFolderPath(null_mesh)), "Error loading null mesh asset!");
			ASSERT(InstantiateTexture(TexturesFolderPath(null_texture)), "Error loading null texture asset!");
			ASSERT(InstantiateMaterial(SchematicsFolderPath(null_material_schematic)), "Error loading null material asset!");
			ASSERT(InstantiateFont(FontsFolderPath(null_font)), "Error loading null font asset!"); // TODO: Create a valid null font
			ASSERT(InstantiateShaderComponent(ShadersFolderPath(null_vert_component)), "Error loading null vertex component asset!"); // TODO: Remove null component references. Store components and reference them in shader programs
			ASSERT(InstantiateShaderComponent(ShadersFolderPath(null_frag_component)), "Error loading null fragment component!"); // TODO: Remove null component references. Store components and reference them in shader programs
			ASSERT(InstantiateShaderComponent(ShadersFolderPath(null_geo_component)), "Error loading null geometry component!"); // #TODO Shader components can be referenced in the shader itself, so just load a null shader schematic
			ASSERT(InstantiateShaderProgram(SchematicsFolderPath(null_shader_schematic)), "Error loading null shader program schematic!");

			const EngineSettings& engineSettings = Settings::GetEngineSettings();
			if (engineSettings.audioEnabled)
			{
				// #TODO Handle loading null sound asset when audio system is disabled.
				// Currently, there is no null asset to fall back on, but audio is no implemented so no references break yet.
				ASSERT(InstantiateSound(SoundsFolderPath(null_sound)), "Error loading null sound asset!");
			}
		}

		void DeleteAllResources()
		{
			for (auto object : s_Meshes)
				delete object.second;

			for (auto object : s_Textures)
				glDeleteTextures(1, &object.second->s_Handle);

			for (auto object : s_Materials)
				delete object.second;

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
		Mesh* InstantiateMesh(const char* meshFilePath)
		{
			if (!meshFilePath)
				return s_Meshes[null_mesh];

			if (FileExists(meshFilePath))
			{
				if (File::LoadModelFileToMeshes(meshFilePath))
				{
					LOG_TRACE("{0} Mesh loaded {1}", __FUNCTION__, meshFilePath);
				}

				// #TODO Mesh files can contain any number of meshes, with any names.
				// Need to handle this more elegantly.
				if (uPtr<char> meshFullFileName = SmartFileName(meshFilePath, true))
				{
					if (MeshExists(meshFullFileName.get()))
					{
						return s_Meshes[meshFullFileName.get()];
					}
				}
			}
			return s_Meshes[null_mesh];
		}

		Texture* InstantiateTexture(const char* textureName)
		{
			if (FileExists(textureName))
			{
				Texture* texture = new Texture();
				texture->s_Handle = Load2DTexture(textureName);
				if (char* fullFileName = File::FullFileName(textureName))
				{
					texture->s_FileName = fullFileName;
					free(fullFileName);
				}

				if (texture->s_Handle != 0)
				{
					s_Textures[texture->s_FileName] = texture;
					return texture;
				}

				delete texture;
				LOG_WARN("{0} Invalid texture handle: {1}", __FUNCTION__, textureName);
			}
			else
			{
				LOG_WARN("{0} Texture not found: {1}", __FUNCTION__, textureName);
			}

			return s_Textures[null_texture];
		}

		Material* InstantiateMaterial(const char* matName)
		{
			Material* material = nullptr;

			if (strcmp(SmartFileExtension(matName).get(), material_schematic_ext) == 0)
			{
				if (FileExists(matName))
				{
					material = new Material();
					Serialization::DeserializeJsonFromFile(matName, *material);
				}
				else if (FileExists(TexturesFolderPath(matName)))
				{
					material = new Material();
					Serialization::DeserializeJsonFromFile(TexturesFolderPath(matName), *material);
				}
				else
				{
					LOG_ERROR("{0} Could not find .msch file named: {1}!", __FUNCTION__, matName);
				}
			}
			else
			{
				LOG_WARN("{0} Not a .msch file: {1}", __FUNCTION__, matName);
			}

			if (!material)
			{
				LOG_WARN("{0} Material not found: {1}", __FUNCTION__, matName);
				return s_Materials[null_material];
			}

			if (char* fullFileName = File::FullFileName(matName))
			{
				s_Materials[fullFileName] = material;
				free(fullFileName);
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
			handle = File::LoadSound(soundPath);

			if (handle != 0)
			{
				if (char* fullFileName = File::FullFileName(soundPath))
				{
					s_Sounds[fullFileName] = handle;
					free(fullFileName);
				}
				return handle;
			}
			else
			{
				return s_Sounds[null_sound];
			}
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
					if (char* fullFileName = File::FullFileName(schematicFile))
					{
						s_ShaderPrograms[fullFileName] = newShaderProgram;
						free(fullFileName);
					}
					newShaderProgram->FindAttributesAndUniforms();
					return newShaderProgram;
				}
				delete newShaderProgram;
			}
			return s_ShaderPrograms[null_shader_schematic];
		}

		ShaderComponent* InstantiateShaderComponent(const char* componentFilePath)
		{
			// ASSERT(componentFilePath && FileExists(componentFilePath), "Cannot return a valid shader component using an invalid file path!");

			uPtr<char> fileExtension = SmartFileExtension(componentFilePath);

			if (strcmp(fileExtension.get(), vertex_shader_ext) == 0)
			{
				if (ShaderComponent* result = ShaderFactory::CreateShaderComponent(GL_VERTEX_SHADER, componentFilePath))
				{
					s_ShaderComponents[componentFilePath] = result;
					return result;
				}
				return s_ShaderComponents[null_vert_component];
			}
			else if (strcmp(fileExtension.get(), fragment_shader_ext) == 0)
			{
				if (ShaderComponent* result = ShaderFactory::CreateShaderComponent(GL_FRAGMENT_SHADER, componentFilePath))
				{
					s_ShaderComponents[componentFilePath] = result;
					return result;
				}
				return s_ShaderComponents[null_frag_component];
			}
			else if (strcmp(fileExtension.get(), geometry_shader_ext) == 0)
			{
				LOG_WARN("Geometry shaders are not yet supported");
				if (ShaderComponent* result = ShaderFactory::CreateShaderComponent(GL_GEOMETRY_SHADER, componentFilePath))
				{
					s_ShaderComponents[componentFilePath] = result;
					return result;
				}
				return s_ShaderComponents[null_geo_component];
			}
			ASSERT(false, "Invalid shader component type from file extension");
			return nullptr;
		}

		Mesh* GetMesh(const char* meshName)
		{
			if (!meshName)
				return s_Meshes[null_mesh];

			if (s_Meshes.find(meshName) != s_Meshes.end())
			{
				return s_Meshes[meshName];
			}
			return InstantiateMesh(MeshesFolderPath(meshName));
		}

		Mesh* GetMeshFromFile(const char* fileName, const char* meshName)
		{
			if (!meshName)
				return s_Meshes[null_mesh];

			if (MeshExists(meshName))
				return s_Meshes[meshName];

			if (Mesh* result = File::LoadMeshInModelByName(MeshesFolderPath(fileName), meshName))
			{
				s_Meshes[meshName] = result;
				return result;
			}
			return s_Meshes[null_mesh];
		}

		Texture* GetTexture(const char* textureName)
		{
			if (!textureName)
				return s_Textures[null_texture];

			if (TextureExists(textureName))
				return s_Textures[textureName];

			if (char* fullFileName = File::FullFileName(textureName))
			{
				if (TextureExists(fullFileName))
				{
					free(fullFileName);
					return s_Textures[fullFileName];
				}
				free(fullFileName);
			}

			return InstantiateTexture(TexturesFolderPath(textureName));
		}

		Texture* GetTextureFromPath(const char* filePath)
		{
			if (!filePath)
				return s_Textures[null_texture];

			if (char* fullFileName = File::FullFileName(filePath))
			{
				if (TextureExists(fullFileName))
				{
					free(fullFileName);
					return s_Textures[fullFileName];
				}
				free(fullFileName);
			}

			return InstantiateTexture(filePath);
		}

		Material* GetMaterial(const char* name)
		{
			if (!name)
				return s_Materials[null_material];

			if (s_Materials.find(name) != s_Materials.end())
				return s_Materials[name];

			return InstantiateMaterial(TexturesFolderPath(name));
		}

		Material* GetMaterialFromPath(const char* filePath)
		{
			if (!filePath)
				return s_Materials[null_material];

			if (char* fullFileName = File::FullFileName(filePath))
			{
				if (MaterialExists(fullFileName))
				{
					// free(fullFileName);
					return s_Materials[fullFileName]; // #TODO Can't free fullFileName
				}
				free(fullFileName);
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

			char* fullFileName = File::FullFileName(filePath);
			if (!fullFileName)
				return s_Fonts[null_font];

			if (s_Fonts.find(fullFileName) != s_Fonts.end())
			{
				free(fullFileName);
				return s_Fonts[fullFileName];
			}

			free(fullFileName);
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

			char* fullFileName = File::FullFileName(filePath);
			if (!fullFileName)
				return s_Sounds[null_sound];

			if (SoundExists(fullFileName))
			{
				free(fullFileName);
				return s_Sounds[fullFileName];
			}
			free(fullFileName);
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

			if (char* fullFileName = File::FullFileName(filePath))
			{
				if (ShaderProgramExists(fullFileName))
				{
					return s_ShaderPrograms[fullFileName];
				}
				else
				{
					return InstantiateShaderProgram(filePath);
				}
				free(fullFileName);
			}
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

			if (char* fullFileName = File::FullFileName(filePath))
			{
				if (ShaderComponentExists(filePath))
				{
					// free(fullFileName);
					return s_ShaderComponents[fullFileName];
				}
				free(fullFileName);
			}

			return InstantiateShaderComponent(filePath);
		}

		const std::map<std::string, Mesh*>& SeeMeshes() { return s_Meshes; };
		const std::map<std::string, Texture*>& SeeTextures() { return s_Textures; };
		const std::map<std::string, Material*>& SeeMaterials() { return s_Materials; };
		const std::map<std::string, ALuint>& SeeSounds() { return s_Sounds; };
		const std::map<std::string, ShaderProgram*>& SeeShaderPrograms() { return s_ShaderPrograms; };
		const std::map<std::string, ShaderComponent*>& SeeShaderComponents() { return s_ShaderComponents; };

		void TextureLoaded(const char* name, int handle)
		{
			if (name) // #TODO Verify handle value?  && handle != 0
			{
				s_Textures[name]->s_Handle = (GLuint)handle;
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
			if (!filePath || mesh == nullptr || mesh->GetName() == gc_DefaultStringValue)
				return false;

			char* fullFileName = File::FullFileName(filePath);
			if (MeshExists(fullFileName))
				return false;

			s_Meshes[fullFileName] = mesh;

			if (fullFileName)
			{
				free(fullFileName);
			}
			return true;
		}

		bool MaterialLoaded(const char* name, Material* material)
		{
			// EarlyReturnIfNull(2, name, material);

			if (!name || !material || material->GetMaterialName() == gc_DefaultStringValue)
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
