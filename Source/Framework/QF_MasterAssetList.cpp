#include "QF_Resources.h"

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

    std::map<std::string, Mesh*> Resources::m_Meshes;
    std::map<std::string, Texture*> Resources::m_Textures;
    std::map<std::string, Material*> Resources::m_Materials;
    std::map<std::string, FT_Face> Resources::m_Fonts; // TODO: Abstract freetype2
    std::map<std::string, ALuint> Resources::m_Sounds; // TODO: Abstract OpenAL
    std::map<std::string, ShaderProgram*> Resources::m_ShaderPrograms;
    std::map<std::string, ShaderComponent*> Resources::m_ShaderComponents;

	void Resources::Initialize()
	{
		InstantiateNullAssets();
	}

	static unsigned int g_UniqueID = 0;
	int Resources::CreateGUID()
	{
		return g_UniqueID++;
	}

	void Resources::InstantiateNullAssets()
	{
		ASSERT(Resources::InstantiateMesh(NullFolderPath(null_mesh)), "Error loading null mesh asset!");
		ASSERT(Resources::InstantiateTexture(NullFolderPath(null_texture)), "Error loading null texture asset!");
		ASSERT(Resources::InstantiateMaterial(NullFolderPath(null_material_schematic)), "Error loading null material asset!");
		ASSERT(Resources::InstantiateFont(NullFolderPath(null_font)), "Error loading null font asset!"); // TODO: Create a valid null font
		ASSERT(Resources::InstantiateShaderComponent(NullFolderPath(null_vert_component)), "Error loading null vertex component asset!"); // TODO: Remove null component references. Store components and reference them in shader programs
		ASSERT(Resources::InstantiateShaderComponent(NullFolderPath(null_frag_component)), "Error loading null fragment component!"); // TODO: Remove null component references. Store components and reference them in shader programs
		ASSERT(Resources::InstantiateShaderComponent(NullFolderPath(null_geo_component)), "Error loading null geometry component!"); // #TODO Shader components can be referenced in the shader itself, so just load a null shader schematic
		ASSERT(Resources::InstantiateShaderProgram(NullFolderPath(null_shader_schematic)), "Error loading null shader program!");

		const EngineSettings& engineSettings = Settings::GetEngineSettings();
		if (engineSettings.audioEnabled)
		{
			// #TODO Handle loading null sound asset when audio system is disabled.
			// Currently, there is no null asset to fall back on, but audio is no implemented so no references break yet.
			ASSERT(Resources::InstantiateSound(NullFolderPath(null_sound)), "Error loading null sound asset!");
		}
	}

	// TODO: Handle errors and deleting assets before returning nullptr
	Mesh* Resources::InstantiateMesh(const char* meshFilePath)
	{
		if (!meshFilePath)
			return m_Meshes[null_mesh];

		Mesh* mesh = nullptr;

		if (FileExists(meshFilePath))
		{
			if (File::LoadModelFileToMeshes(meshFilePath))
			{
				LOG_TRACE("{0} Mesh loaded {1}", __FUNCTION__, meshFilePath);
			}

			char* meshFullFileName = File::FullFileName(meshFilePath);
			if (!meshFullFileName)
				return m_Meshes[null_mesh];

			return GetMesh(meshFullFileName);
			if (MeshExists(meshFullFileName))
			{
				mesh = m_Meshes[meshFullFileName];
			}
			else
			{
				mesh = m_Meshes[null_mesh];
			}
			free(meshFullFileName);
			return mesh;
		}
		else
        {
            if (strcmp(meshFilePath, null_mesh) == 0)
            {
				mesh = MeshFactory::ImportOBJMesh(null_mesh, vec3(0.5f, 0.5f, 0.5f), vec2(1, 1), false);
            }
			else if (strcmp(meshFilePath, MeshesFolderPath("Create_Quad")) == 0)
			{
				mesh = MeshFactory::CreateQuad(vec2(10, 10));
			}
			else if (strcmp(meshFilePath, MeshesFolderPath("Create_Circle")) == 0)
			{
				mesh = MeshFactory::CreateCircle(1.0f, 20, vec2(1, 1));
			}
			else if (strcmp(meshFilePath, MeshesFolderPath("Create_Cube")) == 0)
			{
				mesh = MeshFactory::CreateCube(vec3(1, 1, 1), vec2(1, 1), true);
			}
			else if (strcmp(meshFilePath, MeshesFolderPath("Tutorial_Cube")) == 0)
			{
				mesh = MeshFactory::TutorialCube(vec3(1, 1, 1));
			}
			else if (strcmp(meshFilePath, MeshesFolderPath("Test_Plane")) == 0)
			{
				mesh = MeshFactory::CreateTestPlane();
			}
			// update to use new model loading capabilities
			/*
			else if (meshName == "Teapot.obj")
			{
				mesh = MeshFactory::ImportOBJMesh(MeshPath("Teapot.obj"), vec3(0.5f,0.5f,0.5f), vec2(1,1), false);
			}
			*/
			else
			{
				LOG_ERROR("InstantiateMesh(): Mesh not found! {0}", meshFilePath);
				return m_Meshes[null_mesh];
			}

			m_Meshes[GetFileNameWithExt(meshFilePath).c_str()] = mesh; // Add to active list
			mesh->SetName(GetFileNameWithExt(meshFilePath).c_str());
			return mesh;
		}
	}

	Texture* Resources::InstantiateTexture(const char* textureName)
	{
		if (FileExists(textureName))
		{
			Texture* texture = new Texture();
			texture->s_Handle = Load2DTexture(textureName);
			texture->s_FileName = GetFileNameWithExt(textureName);

			if (texture->s_Handle != 0)
			{
				m_Textures[texture->s_FileName] = texture;
				return texture;
			}

			delete texture;
			return m_Textures[null_texture];
		}

		LOG_WARN("{0} Texture not found: {1}", __FUNCTION__, textureName);
		return m_Textures[null_texture];
	}

	Material* Resources::InstantiateMaterial(const char* matName)
	{
		Material* material = nullptr;

		if (strcmp(GetFileExtension(matName).c_str(), material_schematic_ext) == 0)
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

		m_Materials[GetFileNameWithExt(matName).c_str()] = material;
		return material;
	}

	FT_Face Resources::InstantiateFont(const char* fontName)
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

	ALuint Resources::InstantiateSound(const char* soundPath)
	{
		ALuint handle = 0;
		handle = File::LoadSound(soundPath);

		if (handle != 0)
		{
            m_Sounds[GetFileNameWithExt(soundPath)] = handle;
			return handle;
		}
		else
		{
			return m_Sounds[null_sound];
		}
	}

	ShaderProgram* Resources::InstantiateShaderProgram(const char* schematicFile)
	{
		if (FileExists(schematicFile))
		{
			ShaderProgram* newShaderProgram = new ShaderProgram();
			Serialization::DeserializeJsonFromFile(schematicFile, *newShaderProgram);

			if (newShaderProgram)
			{
				newShaderProgram->SetVertShader(GetShaderComponent(newShaderProgram->GetVertName().c_str(), eShaderComponentTypes::Vertex));
				newShaderProgram->SetFragShader(GetShaderComponent(newShaderProgram->GetFragName().c_str(), eShaderComponentTypes::Fragment));
				// newShaderProgram->s_geoShader = GetShaderComponentData(newShaderProgram->s_geoName.c_str(), eShaderComponentTypes::Geometry);

				if (ShaderFactory::LinkCreatedShaderProgram(newShaderProgram))
				{
					m_ShaderPrograms[GetFileNameWithExt(schematicFile)] = newShaderProgram;
					newShaderProgram->FindAttributesAndUniforms();
					return newShaderProgram;
				}
			}
			delete newShaderProgram;
        }
        return m_ShaderPrograms[null_shader_schematic];
	}

	ShaderComponent* Resources::InstantiateShaderComponent(const char* componentFilePath)
	{
		// ASSERT(componentFilePath && FileExists(componentFilePath), "Cannot return a valid shader component using an invalid file path!");

		if (strcmp(GetFileExtension(componentFilePath).c_str(), vertex_shader_ext) == 0)
		{
			if (ShaderComponent* result = ShaderFactory::CreateShaderComponent(GL_VERTEX_SHADER, componentFilePath))
			{
				m_ShaderComponents[componentFilePath] = result;
				return result;
			}
			return m_ShaderComponents[null_vert_component];
		}
		else if (strcmp(GetFileExtension(componentFilePath).c_str(), fragment_shader_ext) == 0)
		{
			if (ShaderComponent* result = ShaderFactory::CreateShaderComponent(GL_FRAGMENT_SHADER, componentFilePath))
			{
				m_ShaderComponents[componentFilePath] = result;
				return result;
			}
			return m_ShaderComponents[null_frag_component];
		}
		else if (strcmp(GetFileExtension(componentFilePath).c_str(), geometry_shader_ext) == 0)
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
