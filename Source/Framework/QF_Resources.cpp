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
		// m_Fonts.clear(); // #TODO Delete fonts
	}

	bool Resources::MeshExists(const char* name)
	{
		return m_Meshes.find(name) != m_Meshes.end();
	}

	bool Resources::TextureExists(const char* name)
	{
		return m_Textures.find(name) != m_Textures.end();
	}

	bool Resources::MaterialExists(const char* name)
	{
		return m_Materials.find(name) != m_Materials.end();
	}

	bool Resources::FontExists(const char* name)
	{
		return m_Fonts.find(name) != m_Fonts.end();
	}

	bool Resources::SoundExists(const char* name)
	{
		return m_Sounds.find(name) != m_Sounds.end();
	}

	bool Resources::ShaderProgramExists(const char* name)
	{
		return m_ShaderPrograms.find(name) != m_ShaderPrograms.end();
	}

	bool Resources::ShaderComponentExists(const char* name)
	{
		return m_ShaderComponents.find(name) != m_ShaderComponents.end();
	}

	bool Resources::AddMesh(const char* name, Mesh* mesh)
	{
		if (mesh == nullptr || mesh->GetName() == gc_DefaultStringValue)
			return false;

		if (MeshExists(name))
			return false;

		m_Meshes[name] = mesh;
		return true;
	}

	bool Resources::AddTexture(const char* name, Texture* texture)
	{
		if (!name || !texture)
			return false;

		if (TextureExists(name))
			return false;

		if (texture->s_Handle == 0)
			return false;

		m_Textures[name] = texture;
		return true;
	}

	bool Resources::AddMaterial(const char* name, Material* material)
	{
		// EarlyReturnIfNull(2, name, material);

		if (!name || !material || MaterialExists(name))
			return false;

		if (material->GetMaterialName() == gc_DefaultStringValue)
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

	bool Resources::AddShaderProgram(const char* name, ShaderProgram* ShaderProgram)
	{
		if (!name || ShaderProgram == nullptr)
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
		{
			return m_Meshes[meshName];
		}

		Mesh* result = File::LoadMeshInModelByName(MeshesFolderPath(fileName), meshName);
		if (result)
		{
			m_Meshes[meshName] = result;
			return result;
		}
		return m_Meshes[null_mesh];
	}

	void Resources::UpdateTexture(const char* name, int handle)
	{
		if (name)
		{
			m_Textures[name]->s_Handle = (GLuint)handle;
		}
	}

	Texture* Resources::GetTexture(const char* name)
	{
		if (!name)
			return m_Textures[null_texture];

		if (m_Textures.find(name) != m_Textures.end())
			return m_Textures[name];

		// TODO: After a texture has been requested, create an entry in the map with null data.
		// When the data is loaded, replace the null data.
		// This is too avoid multiple loading of the same assets.

		//* <-- toggle double slash
		else
		{
			// NOTE: Auto loading should be avoided. Callers expecting to load a file should check if it exists already.
			if (FileExists(TexturesFolderPath(name)))
			{
				Texture* tex = new Texture();
				tex->s_Handle = m_Textures[null_texture]->s_Handle;
				tex->s_FileName = GetFileNameWithExt(name);
				m_Textures[tex->s_FileName] = tex;

				Jobs::ScheduleTask(new QLoadAsset(name));
				return m_Textures[tex->s_FileName];
			}
			else
			{
				return m_Textures[null_texture];
			}
		}
		/*/
		return InstantiateTexture(TexturesFolderPath(name));
		// */
		return nullptr;
	}

	Texture* Resources::GetTextureFromPath(const char* filePath)
	{
		if (!filePath)
			return m_Textures[null_texture];

		if (TextureExists(GetFileNameWithExt(filePath).c_str()))
			return m_Textures[GetFileNameWithExt(filePath).c_str()];

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

		if (m_Materials.find(GetFileNameWithExt(filePath).c_str()) != m_Materials.end())
			return m_Materials[GetFileNameWithExt(filePath).c_str()];

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

		if (m_Fonts.find(GetFileNameWithExt(filePath).c_str()) != m_Fonts.end())
			return m_Fonts[GetFileNameWithExt(filePath).c_str()];

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

		if (SoundExists(GetFileNameWithExt(filePath).c_str()))
			return m_Sounds[GetFileNameWithExt(filePath).c_str()];

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

		if (ShaderProgramExists(GetFileNameWithExt(filePath).c_str()))
			return m_ShaderPrograms[GetFileNameWithExt(filePath).c_str()];

		return InstantiateShaderProgram(filePath);
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

		if (ShaderComponentExists(GetFileNameWithExt(filePath).c_str()))
			return m_ShaderComponents[GetFileNameWithExt(filePath).c_str()];

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
