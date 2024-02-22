#include "QF_Assets.h"

#include "QC_StringHelpers.h"

#include "QF_Debug.h"
#include "QF_Material.h"
#include "QF_Settings.h"
#include "QF_Serialization.h"
#include "QF_ShaderProgram.h"
#include "QF_ShaderComponent.h"
#include "QF_ShaderFactory.h"
#include "QF_FileSystem.h"
#include "QF_Defines.h"
#include "QF_Texture.h"
#include "QF_Jobs.h"
#include "QF_Log.h"

namespace QwerkE {

    std::map<std::string, Mesh*> Assets::m_Meshes;
    std::map<std::string, Texture*> Assets::m_Textures;
    std::map<std::string, Material*> Assets::m_Materials;
    std::map<std::string, FT_Face> Assets::m_Fonts; // TODO: Abstract freetype2
    std::map<std::string, ALuint> Assets::m_Sounds; // TODO: Abstract OpenAL
    std::map<std::string, ShaderProgram*> Assets::m_ShaderPrograms;
    std::map<std::string, ShaderComponent*> Assets::m_ShaderComponents;

	void Assets::Initialize()
	{
		ASSERT(m_Meshes.empty() && m_Textures.empty() &&
				m_Materials.empty() && m_Fonts.empty() &&
				m_Sounds.empty() && m_ShaderPrograms.empty() &&
				m_ShaderComponents.empty(),
				"Resources already initialized!");

		ASSERT(Assets::InstantiateMesh(MeshesFolderPath(null_mesh)), "Error loading null mesh asset!");
		ASSERT(Assets::InstantiateTexture(TexturesFolderPath(null_texture)), "Error loading null texture asset!");
		ASSERT(Assets::InstantiateMaterial(SchematicsFolderPath(null_material_schematic)), "Error loading null material asset!");
		ASSERT(Assets::InstantiateFont(FontsFolderPath(null_font)), "Error loading null font asset!"); // TODO: Create a valid null font
		ASSERT(Assets::InstantiateShaderComponent(ShadersFolderPath(null_vert_component)), "Error loading null vertex component asset!"); // TODO: Remove null component references. Store components and reference them in shader programs
		ASSERT(Assets::InstantiateShaderComponent(ShadersFolderPath(null_frag_component)), "Error loading null fragment component!"); // TODO: Remove null component references. Store components and reference them in shader programs
		ASSERT(Assets::InstantiateShaderComponent(ShadersFolderPath(null_geo_component)), "Error loading null geometry component!"); // #TODO Shader components can be referenced in the shader itself, so just load a null shader schematic
		ASSERT(Assets::InstantiateShaderProgram(SchematicsFolderPath(null_shader_schematic)), "Error loading null shader program schematic!");

		const EngineSettings& engineSettings = Settings::GetEngineSettings();
		if (engineSettings.audioEnabled)
		{
			// #TODO Handle loading null sound asset when audio system is disabled.
			// Currently, there is no null asset to fall back on, but audio is no implemented so no references break yet.
			ASSERT(Assets::InstantiateSound(SoundsFolderPath(null_sound)), "Error loading null sound asset!");
		}
	}

	static unsigned int g_UniqueID = 0;
	int Assets::CreateGUID() // #FEATURE F0004 - GUIDs
	{
		return g_UniqueID++;
	}

	// TODO: Handle errors and deleting assets before returning nullptr
	Mesh* Assets::InstantiateMesh(const char* meshFilePath)
	{
		if (!meshFilePath)
			return m_Meshes[null_mesh];

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
					return m_Meshes[meshFullFileName.get()];
				}
			}
		}
		return m_Meshes[null_mesh];
	}

	Texture* Assets::InstantiateTexture(const char* textureName)
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
				m_Textures[texture->s_FileName] = texture;
				return texture;
			}

			delete texture;
			LOG_WARN("{0} Invalid texture handle: {1}", __FUNCTION__, textureName);
		}
		else
		{
			LOG_WARN("{0} Texture not found: {1}", __FUNCTION__, textureName);
		}

		return m_Textures[null_texture];
	}

	Material* Assets::InstantiateMaterial(const char* matName)
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
			return m_Materials[null_material];
		}

		if (char* fullFileName = File::FullFileName(matName))
		{
			m_Materials[fullFileName] = material;
			free(fullFileName);
		}
		return material;
	}

	FT_Face Assets::InstantiateFont(const char* fontName)
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
			return m_Fonts[null_font];
		}
		m_Fonts[fontName] = font;
		FT_Done_FreeType(ft);
		assert(font != NULL);
		return font;
	}

	ALuint Assets::InstantiateSound(const char* soundPath)
	{
		ALuint handle = 0;
		handle = File::LoadSound(soundPath);

		if (handle != 0)
		{
			if (char* fullFileName = File::FullFileName(soundPath))
			{
				m_Sounds[fullFileName] = handle;
				free(fullFileName);
			}
			return handle;
		}
		else
		{
			return m_Sounds[null_sound];
		}
	}

	ShaderProgram* Assets::InstantiateShaderProgram(const char* schematicFile)
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
					m_ShaderPrograms[fullFileName] = newShaderProgram;
					free(fullFileName);
				}
				newShaderProgram->FindAttributesAndUniforms();
				return newShaderProgram;
			}
			delete newShaderProgram;
        }
        return m_ShaderPrograms[null_shader_schematic];
	}

	ShaderComponent* Assets::InstantiateShaderComponent(const char* componentFilePath)
	{
		// ASSERT(componentFilePath && FileExists(componentFilePath), "Cannot return a valid shader component using an invalid file path!");

		uPtr<char> fileExtension = SmartFileExtension(componentFilePath);

		if (strcmp(fileExtension.get(), vertex_shader_ext) == 0)
		{
			if (ShaderComponent* result = ShaderFactory::CreateShaderComponent(GL_VERTEX_SHADER, componentFilePath))
			{
				m_ShaderComponents[componentFilePath] = result;
				return result;
			}
			return m_ShaderComponents[null_vert_component];
		}
		else if (strcmp(fileExtension.get(), fragment_shader_ext) == 0)
		{
			if (ShaderComponent* result = ShaderFactory::CreateShaderComponent(GL_FRAGMENT_SHADER, componentFilePath))
			{
				m_ShaderComponents[componentFilePath] = result;
				return result;
			}
			return m_ShaderComponents[null_frag_component];
		}
		else if (strcmp(fileExtension.get(), geometry_shader_ext) == 0)
		{
			LOG_WARN("Geometry shaders are not yet supported");
			if (ShaderComponent* result = ShaderFactory::CreateShaderComponent(GL_GEOMETRY_SHADER, componentFilePath))
			{
				m_ShaderComponents[componentFilePath] = result;
				return result;
			}
			return m_ShaderComponents[null_geo_component];
		}
		ASSERT(false, "Invalid shader component type from file extension");
		return nullptr;
	}

}