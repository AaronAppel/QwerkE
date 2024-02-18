#pragma once

#include <string>
#include <map>
#include <memory>

#include "Libraries/glew/GL/glew.h"
#include "Libraries/freetype2/ft2build.h" // #TODO Temp build fix
#include "Libraries/freetype2/freetype/freetype.h"
#include "Libraries/OpenAL/include/al.h"

#include "QF_Constants.h"
#include "QF_Graphics_Header.h"

// For instances where 1 asset may be shared between objects in
// possibly many scenes, the Resources() may want to count
// the number of instances of an asset and only off load any
// memory when all objects are finished with that asset.

namespace QwerkE {

    class Material;
    class Mesh;
    class ShaderComponent;
    class ShaderProgram;

    struct Texture;

    class Resources final // #TODO Consider for static namespace
    {
    public:
        static void Initialize();

        static int CreateGUID();

        static bool MeshExists(const char* name);
        static bool TextureExists(const char* name);
        static bool MaterialExists(const char* name);
        static bool FontExists(const char* name);
        static bool SoundExists(const char* name);
        static bool ShaderProgramExists(const char* name);
        static bool ShaderComponentExists(const char* name);

        static bool AddMesh(const char* filePath, Mesh* mesh); // #TODO Change to get name from object itself
        static bool AddTexture(const char* name, Texture* texture); // #TODO Change to use file name of texture itself
        static bool AddMaterial(const char* name, Material* material); // #TODO Change to get name from object itself
        static bool AddFont(const char* name, FT_Face font);
        static bool AddSound(const char* name, ALuint sound);
        static bool AddShaderProgram(ShaderProgram* shaderProgram);
        static bool AddShaderProgram(const char* name, ShaderProgram* ShaderProgram); // #TODO Change to get name from object itself
        static bool AddShaderComponent(const char* name, ShaderComponent* shaderComponent); // #TODO Change to get name from object itself
        // #TODO Other add functions

        // getters
        // #TODO return const*s so they cannot be modified externally
        // The following functions guarantee a valid return variable using null or error objects
        // #TODO Should getters generate objects? What would be a better way?
        static Mesh* GetMesh(const char* name); // specify .ext
        static Mesh* GetMeshFromFile(const char* filePath, const char* meshName); // specify .ext
        static void UpdateTexture(const char* name, int handle); // TESTING:
        static Texture* GetTexture(const char* name); // specify .ext
        static Texture* GetTextureFromPath(const char* filePath);
        static Material* GetMaterial(const char* name); // specify .ext
        static Material* GetMaterialFromPath(const char* filePath);
        static FT_Face GetFont(const char* name); // specify .ext
        static FT_Face GetFontFromPath(const char* filePath);
        static ALuint GetSound(const char* name); // specify .ext
        static ALuint GetSoundFromPath(const char* filePath);
        static ShaderProgram* GetShaderProgram(const char* name); // #TODO specify .ext
        static ShaderProgram* GetShaderProgramFromPath(const char* filePath);
        static ShaderComponent* GetShaderComponent(const char* name, eShaderComponentTypes componentType);
        static ShaderComponent* GetShaderComponentFromPath(const char* filePath, eShaderComponentTypes componentType);

        static const std::map<std::string, Mesh*>& SeeMeshes() { return m_Meshes; };
        static const std::map<std::string, Texture*>& SeeTextures() { return m_Textures; };
        static const std::map<std::string, Material*>& SeeMaterials() { return m_Materials; };
        static const std::map<std::string, ALuint>& SeeSounds() { return m_Sounds; };
        static const std::map<std::string, ShaderProgram*>& SeeShaderPrograms() { return m_ShaderPrograms; };
        static const std::map<std::string, ShaderComponent*>& SeeShaderComponents() { return m_ShaderComponents; };

        // #TODO Handle loading additional resources
        // CubeMap* GetCubeMap(const char* name); // specify .ext
        // FBO* GetFBO(const char* name); // specify .ext
        // Sound* GetSound(const char* name); // specify .ext

        // Load directory
        // void LoadDirectory(const char* folder); // #TODO Move outside and into a File class method

        // Cleanup
        static void DeleteAllResources();

    private:
        Resources() = default;
        ~Resources();

        static std::map<std::string, std::shared_ptr<Mesh*>> m_MeshesSmartPointers; // #TODO Try using smart pointers

        // #TODO Consider using unordered maps or linked lists. Review data structure choice
        static std::map<std::string, Mesh*> m_Meshes;
        static std::map<std::string, Texture*> m_Textures;
        static std::map<std::string, Material*> m_Materials;
        static std::map<std::string, FT_Face> m_Fonts; // #TODO Abstract freetype2
        static std::map<std::string, ALuint> m_Sounds; // #TODO Abstract OpenAL
        static std::map<std::string, ShaderProgram*> m_ShaderPrograms;
        static std::map<std::string, ShaderComponent*> m_ShaderComponents;

        static bool IsUnique(Mesh* mesh);
        static bool IsUnique(Texture* texturehandle);
        static bool IsUnique(Material* material);
        static bool IsUnique(FT_Face font);
        static bool IsSoundUnique(ALuint sound);
        static bool IsShaderProgramUnique(ShaderProgram* ShaderProgram);
        static bool IsShaderComponentsUnique(ShaderComponent* shaderComponents);

        static Mesh* InstantiateMesh(const char* name);
        static Texture* InstantiateTexture(const char* name);
        static Material* InstantiateMaterial(const char* name);
        static FT_Face InstantiateFont(const char* fontName);
        static ALuint InstantiateSound(const char* soundName);
        static ShaderProgram* InstantiateShaderProgram(const char* schematicName);
        static ShaderComponent* InstantiateShaderComponent(const char* componentName);

        // #TODO void* return? Actually write functions.
        // Remember that smart pointers may be necessary for references.
        static bool DeleteMesh(const char* name) {}
        static bool DeleteTexture(const char* name) {}
        static bool DeleteMaterial(const char* name) {}
        static bool DeleteFont(const char* name) {}
        static bool DeleteSound(const char* name) {}
        static bool DeleteShaderProgram(const char* name) {}
        static bool DeleteShaderComponent(const char* name) {}
    };

}
