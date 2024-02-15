#include "QF_Resources.h"

#include <map>
#include <memory>

#include "Libraries/glew/GL/glew.h"

#include "QF_Debug.h"
#include "QF_Defines.h"
#include "QF_FileSystem.h"
#include "QF_Jobs.h"
#include "QF_Material.h"
#include "QF_Mesh.h"
#include "QF_ShaderProgram.h"
#include "QF_Texture.h"

namespace QwerkE {

	Resources::~Resources()
	{
		DeleteAllResources();
	}

	void Resources::DeleteAllResources()
    {
		for (auto object : m_Meshes)
			delete object.second;

		for (auto object : m_Textures)
			glDeleteTextures(1, &object.second->s_Handle);

		for (auto object : m_Materials)
			delete object.second;

		m_Meshes.clear();
		m_Textures.clear();
		m_Materials.clear();
		m_Fonts.clear();
	}

	bool Resources::MeshExists(const char* name)
	{
		return m_Meshes.find(name) != m_Meshes.end();
	}

	bool Resources::TextureExists(const char* name)
	{
		if (!name) return false;
		return m_Textures.find(name) != m_Textures.end();
	}

	bool Resources::MaterialExists(const char* name)
	{
		if (!name) return false;
		return m_Materials.find(name) != m_Materials.end();
	}

	bool Resources::FontExists(const char* name)
	{
		if (!name) return false;
		return m_Fonts.find(name) != m_Fonts.end();
	}

	bool Resources::SoundExists(const char* name)
	{
		if (!name) return false;
		return m_Sounds.find(name) != m_Sounds.end();
	}

	bool Resources::ShaderProgramExists(const char* name)
	{
		if (!name) return false;
		return m_ShaderPrograms.find(name) != m_ShaderPrograms.end();
	}

	bool Resources::ShaderComponentExists(const char* name)
	{
		if (!name) return false;
		return m_ShaderComponents.find(name) != m_ShaderComponents.end();
	}

	bool Resources::AddMesh(const char* filePath, Mesh* mesh)
	{
		if (!filePath || mesh == nullptr || mesh->GetName() == gc_DefaultStringValue)
			return false;

		char* fullFileName = File::FullFileName(filePath);
		if (MeshExists(fullFileName))
			return false;

		m_Meshes[fullFileName] = mesh;

		if (fullFileName)
		{
			free(fullFileName);
		}
		return true;
	}

	bool Resources::AddTexture(const char* name, Texture* texture)
	{
		if (!name || !texture || texture->s_Handle == 0)
			return false;

		if (TextureExists(name))
			return false;

		m_Textures[name] = texture;
		return true;
	}

	bool Resources::AddMaterial(const char* name, Material* material)
	{
		// EarlyReturnIfNull(2, name, material);

		if (!name || !material || material->GetMaterialName() == gc_DefaultStringValue)
			return false;

		if (MaterialExists(name))
			return false;

		m_Materials[name] = material;
		return true;
	}

	bool Resources::AddFont(const char* name, FT_Face font)
	{
		if (!name || !font || FontExists(name))
			return false;

		// TODO: How to check for null font? Also need cross library font classes
		// if (font == nullptr || material->name == gc_DefaultStringValue)
			// return false;

		m_Fonts[name] = font;
		return true;
	}

	bool Resources::AddSound(const char* name, ALuint sound)
	{
		if (!name || sound == 0)
			return false;

		if (SoundExists(name))
			return false;

		m_Sounds[name] = sound;
		return true;
	}

	bool Resources::AddShaderProgram(ShaderProgram* shaderProgram)
	{
		if (!shaderProgram || !shaderProgram->GetName().c_str())
			return false;

		return AddShaderProgram(shaderProgram->GetName().c_str(), shaderProgram);
	}

	bool Resources::AddShaderProgram(const char* name, ShaderProgram* ShaderProgram)
	{
		if (!name || !ShaderProgram)
			return false;

		if (ShaderProgramExists(name))
			return false;

		m_ShaderPrograms[name] = ShaderProgram;
		return true;
	}

	bool Resources::AddShaderComponent(const char* name, ShaderComponent* shaderComponent)
	{
		if (!name || shaderComponent == nullptr)
			return false;

		if (ShaderComponentExists(name))
			return false;

		m_ShaderComponents[name] = shaderComponent;
		return true;
	}

	Mesh* Resources::GetMesh(const char* meshName)
	{
		if (!meshName)
			return m_Meshes[null_mesh];

		if (m_Meshes.find(meshName) != m_Meshes.end())
		{
			return m_Meshes[meshName];
		}
		return InstantiateMesh(MeshesFolderPath(meshName));
	}

	Mesh* Resources::GetMeshFromFile(const char* fileName, const char* meshName)
	{
		if (!meshName)
			return m_Meshes[null_mesh];

		if (MeshExists(meshName))
			return m_Meshes[meshName];

		if (Mesh* result = File::LoadMeshInModelByName(MeshesFolderPath(fileName), meshName))
		{
			m_Meshes[meshName] = result;
			return result;
		}
		return m_Meshes[null_mesh];
	}

	void Resources::UpdateTexture(const char* name, int handle)
	{
		if (name) // #TODO Verify handle value?  && handle != 0
		{
			m_Textures[name]->s_Handle = (GLuint)handle;
		}
	}

	Texture* Resources::GetTexture(const char* textureName)
	{
		if (!textureName)
			return m_Textures[null_texture];

		if (TextureExists(textureName))
			return m_Textures[textureName];

		if (char* fullFileName = File::FullFileName(textureName))
		{
			if (TextureExists(fullFileName))
			{
				free(fullFileName);
				return m_Textures[fullFileName];
			}
			free(fullFileName);
		}

		return InstantiateTexture(TexturesFolderPath(textureName));
	}

	Texture* Resources::GetTextureFromPath(const char* filePath)
	{
		if (!filePath)
			return m_Textures[null_texture];

		if (char* fullFileName = File::FullFileName(filePath))
		{
			if (TextureExists(fullFileName))
			{
				free(fullFileName);
				return m_Textures[fullFileName];
			}
			free(fullFileName);
		}

		return InstantiateTexture(filePath);
	}

	Material* Resources::GetMaterial(const char* name)
	{
		if (!name)
			return m_Materials[null_material];

		if (m_Materials.find(name) != m_Materials.end())
			return m_Materials[name];

		return InstantiateMaterial(TexturesFolderPath(name));
	}

	Material* Resources::GetMaterialFromPath(const char* filePath)
	{
		if (!filePath)
			return m_Materials[null_material];

		if (char* fullFileName = File::FullFileName(filePath))
		{
			if (MaterialExists(fullFileName))
			{
				// free(fullFileName);
				return m_Materials[fullFileName]; // #TODO Can't free fullFileName
			}
			free(fullFileName);
		}

		return InstantiateMaterial(filePath);
	}

	FT_Face Resources::GetFont(const char* name)
	{
		if (!name)
			return m_Fonts[null_font];

		if (m_Fonts.find(name) != m_Fonts.end())
			return m_Fonts[name];

		return InstantiateFont(FontsFolderPath(name));
	}

	FT_Face Resources::GetFontFromPath(const char* filePath)
	{
		if (!filePath)
			return m_Fonts[null_font];

		char* fullFileName = File::FullFileName(filePath);
		if (!fullFileName)
			return m_Fonts[null_font];

		if (m_Fonts.find(fullFileName) != m_Fonts.end())
		{
			free(fullFileName);
			return m_Fonts[fullFileName];
		}

		free(fullFileName);
		return InstantiateFont(filePath);
	}

	ALuint Resources::GetSound(const char* name)
	{
		if (!name)
			return m_Sounds[null_sound];

		if (SoundExists(name))
			return m_Sounds[name];

		return InstantiateSound(SoundsFolderPath(name));
	}

	ALuint Resources::GetSoundFromPath(const char* filePath)
	{
		if (!filePath)
			return m_Sounds[null_sound];

		char* fullFileName = File::FullFileName(filePath);
		if (!fullFileName)
			return m_Sounds[null_sound];

		if (SoundExists(fullFileName))
		{
			free(fullFileName);
			return m_Sounds[fullFileName];
		}
		free(fullFileName);
		return InstantiateSound(filePath);
	}

	ShaderProgram* Resources::GetShaderProgram(const char* name)
	{
		if (!name)
			return m_ShaderPrograms[null_shader];

		if (ShaderProgramExists(name))
			return m_ShaderPrograms[name];

		return InstantiateShaderProgram(ShadersFolderPath(name));
	}

	ShaderProgram* Resources::GetShaderProgramFromPath(const char* filePath)
	{
		if (!filePath)
			return m_ShaderPrograms[null_shader];

		if (char* fullFileName = File::FullFileName(filePath))
		{
			if (ShaderProgramExists(fullFileName))
			{
				return m_ShaderPrograms[fullFileName];
			}
			else
			{
				return InstantiateShaderProgram(filePath);
			}
			free(fullFileName);
		}
		return m_ShaderPrograms[null_shader];
	}

	ShaderComponent* Resources::GetShaderComponent(const char* name, eShaderComponentTypes componentType)
	{
		if (!name)
		{
			switch (componentType)
			{
			case QwerkE::Vertex:
				return m_ShaderComponents[StringAppend(null_shader, vertex_shader_ext)];
				break;

			case QwerkE::Fragment:
				return m_ShaderComponents[StringAppend(null_shader, fragment_shader_ext)];
				break;

			case QwerkE::Geometry:
				return m_ShaderComponents[StringAppend(null_shader, geometry_shader_ext)];
				break;

			default:
				ASSERT(false, "Cannot determine return shader component for null filePath!");
				break;
			}
		}

		if (ShaderComponentExists(name))
			return m_ShaderComponents[name];

		return InstantiateShaderComponent(ShadersFolderPath(name));
	}

	ShaderComponent* Resources::GetShaderComponentFromPath(const char* filePath, eShaderComponentTypes componentType)
	{
		if (!filePath)
		{
			switch (componentType)
			{
			case QwerkE::Vertex:
				return m_ShaderComponents[StringAppend(null_shader, vertex_shader_ext)];
				break;

			case QwerkE::Fragment:
				return m_ShaderComponents[StringAppend(null_shader, fragment_shader_ext)];
				break;

			case QwerkE::Geometry:
				return m_ShaderComponents[StringAppend(null_shader, geometry_shader_ext)];
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
				return m_ShaderComponents[fullFileName];
			}
			free(fullFileName);
		}

		return InstantiateShaderComponent(filePath);
	}

	bool Resources::IsUnique(Mesh* mesh)
	{
		if (!mesh)
			return false;

		std::map<std::string, Mesh*>::iterator it;
		for (it = m_Meshes.begin(); it != m_Meshes.end(); it++)
		{
			if (it->second == mesh)
				return false;
		}
		return true;
	}

	bool Resources::IsUnique(Texture* texturehandle)
	{
		if (!texturehandle)
			return false;

		std::map<std::string, Texture*>::iterator it;
		for (it = m_Textures.begin(); it != m_Textures.end(); it++)
		{
			if (it->second == texturehandle)
				return false;
		}
		return true;
	}

	bool Resources::IsUnique(Material* material)
	{
		if (!material)
			return false;

		std::map<std::string, Material*>::iterator it;
		for (it = m_Materials.begin(); it != m_Materials.end(); it++)
		{
			if (it->second == material)
				return false;
		}
		return true;
	}

	bool Resources::IsUnique(FT_Face font)
	{
		std::map<std::string, FT_Face>::iterator it;
		for (it = m_Fonts.begin(); it != m_Fonts.end(); it++)
		{
			if (it->second == font)
				return false;
		}
		return true;
	}

	bool Resources::IsSoundUnique(ALuint sound)
	{
		std::map<std::string, ALuint>::iterator it;
		for (it = m_Sounds.begin(); it != m_Sounds.end(); it++)
		{
			if (it->second == sound)
				return false;
		}
		return true;
	}

	bool Resources::IsShaderProgramUnique(ShaderProgram* shaderProgram)
	{
		if (!shaderProgram)
			return false;

		std::map<std::string, ShaderProgram*>::iterator it;
		for (it = m_ShaderPrograms.begin(); it != m_ShaderPrograms.end(); it++)
		{
			if (it->second == shaderProgram)
				return false;
		}
		return true;
	}

	bool Resources::IsShaderComponentsUnique(ShaderComponent* shaderComponent)
	{
		if (!shaderComponent)
			return false;

		std::map<std::string, ShaderComponent*>::iterator it;
		for (it = m_ShaderComponents.begin(); it != m_ShaderComponents.end(); it++)
		{
			if (it->second == shaderComponent)
				return false;
		}
		return true;
	}

}
