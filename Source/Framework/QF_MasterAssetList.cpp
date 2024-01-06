#include "QF_Resources.h"

#include "QC_StringHelpers.h"

#include "QF_Material.h"
#include "QF_ShaderProgram.h"
#include "QF_ShaderComponent.h"
#include "QF_ShaderFactory.h"
#include "QF_FileSystem.h"
#include "QF_QwerkE_Defines.h"
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

	void Resources::InstantiateNullAssets()
	{
		ASSERT(Resources::InstantiateMesh(NullFolderPath(null_mesh_filename)), "Error loading null mesh asset!");
		ASSERT(Resources::InstantiateTexture(NullFolderPath(null_texture)), "Error loading null texture asset!");
		ASSERT(Resources::InstantiateMaterial(NullFolderPath(null_material_schematic)), "Error loading null material asset!");
		ASSERT(Resources::InstantiateFont(NullFolderPath(null_font)), "Error loading null font asset!"); // TODO: Create a valid null font
		ASSERT(Resources::InstantiateMesh(NullFolderPath(null_mesh_filename)), "Error loading null mesh asset!");
		ASSERT(Resources::InstantiateShaderComponent(NullFolderPath(null_vert_component)), "Error loading null vertex component asset!"); // TODO: Remove null component references. Store components and reference them in shader programs
		ASSERT(Resources::InstantiateShaderComponent(NullFolderPath(null_frag_component)), "Error loading null fragment component!"); // TODO: Remove null component references. Store components and reference them in shader programs
		ASSERT(Resources::InstantiateShaderComponent(NullFolderPath(null_geo_component)), "Error loading null geometry component!"); // #TODO Shader components can be referenced in the shader itself, so just load a null shader schematic
		ASSERT(Resources::InstantiateShaderProgram(NullFolderPath(null_shader_schematic)), "Error loading null shader program!");
		ASSERT(Resources::InstantiateSound(NullFolderPath(null_sound)), "Error loading null sound asset!");
	}

	// TODO: Handle errors and deleting assets before returning nullptr
	Mesh* Resources::InstantiateMesh(const char* meshFilePath)
	{
		Mesh* mesh = nullptr;

		if (FileExists(meshFilePath))
		{
			FileSystem::LoadModelFileToMeshes(meshFilePath);
			if (MeshExists(GetFileNameNoExt(meshFilePath).c_str())) // TODO: Better way to handle file paths and resource names
				return m_Meshes[GetFileNameNoExt(meshFilePath).c_str()];
			else
				return m_Meshes[null_mesh];
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
			Texture* texture = nullptr;
			texture = new Texture();
			texture->s_Handle = Load2DTexture(textureName);
			texture->s_FileName = GetFileNameWithExt(textureName);

			if (texture->s_Handle != 0)
			{
				m_Textures[texture->s_FileName] = texture;
				return texture;
			}
			else
			{
				delete texture;
				return m_Textures[null_texture];
			}
		}
		else
		{
			LOG_WARN("InstantiateTexture(): Texture not found: {0}", textureName);
            return m_Textures[null_texture];
		}
	}

	Material* Resources::InstantiateMaterial(const char* matName)
	{
		Material* material = nullptr;

		// TODO: Set null data for handles and names like "Empty"
		if (strcmp(GetFileExtension(matName).c_str(), material_schematic_ext) == 0)
		{
			// TODO: Handle null or corrupt data
			if (FileExists(matName))
				material = LoadMaterialSchematic(matName);
			else
				material = LoadMaterialSchematic(TexturesFolderPath(matName));
		}
		else
        {
			LOG_WARN("InstantiateMaterial(): Not a .msch file: {0}", matName);
		}

		if (!material)
        {
            LOG_WARN("InstantiateMaterial(): Material not found: {0}", matName);
			return m_Materials[null_material]; // Do not add another material
		}
		else
        {
            m_Materials[GetFileNameWithExt(matName).c_str()] = material;
            return material;
		}
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
		handle = FileSystem::LoadSound(soundPath);

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
			ShaderProgram* result = LoadShaderSchematic(schematicFile); // TODO: Free memory on error
			if (result)
			{
				result->SetVertShader(GetShaderComponent(result->GetVertName().c_str()));
				result->SetFragShader(GetShaderComponent(result->GetFragName().c_str()));
				// result->s_geoShader = GetShaderComponentData(result->s_geoName.c_str());

				if (ShaderFactory::LinkCreatedShaderProgram(result))
				{
					m_ShaderPrograms[GetFileNameWithExt(schematicFile)] = result;
					result->FindAttributesAndUniforms();
					return result;
				}
			}
        }
        return m_ShaderPrograms[null_shader_schematic];
	}

	ShaderComponent* Resources::InstantiateShaderComponent(const char* componentFilePath)
	{
		ASSERT(componentFilePath && FileExists(componentFilePath), "Cannot return a valid shader component using an invalid file path!");

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
			LOG_TRACE("Geometry shaders are not yet supported");
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
