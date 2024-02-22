                                                                                                               #pragma once
#include <string>
#include <map>

// #include "Libraries/glew/GL/glew.h"
#include "Libraries/freetype2/ft2build.h" // #TODO Temp build fix
#include "Libraries/freetype2/freetype/freetype.h"
// #include "Libraries/OpenAL/include/al.h"
//
// #include "QF_Constants.h"
#include "QF_Graphics_Header.h"

// For instances where 1 asset may be shared between objects in
// possibly many scenes, the Resources() may want to count
// the number of instances of an asset and only off load any
// memory when all objects are finished with that asset.

typedef unsigned int ALuint;

namespace QwerkE {

    class Material;
    class Mesh;
    class ShaderComponent;
    class ShaderProgram;

    struct Texture;

    namespace Assets {

        void Initialize();
        void DeleteAllResources();

        bool MeshExists(const char* name);
        bool TextureExists(const char* name);
        bool MaterialExists(const char* name);
        bool FontExists(const char* name);
        bool SoundExists(const char* name);
        bool ShaderProgramExists(const char* name);
        bool ShaderComponentExists(const char* name);

        // #TODO Deprecate and own asset creation inside of Assets
        int GetGUID();

        Mesh* GetMesh(const char* name); // specify .ext
        Mesh* GetMeshFromFile(const char* filePath, const char* meshName); // specify .ext
        Texture* GetTexture(const char* name); // specify .ext
        Texture* GetTextureFromPath(const char* filePath);
        Material* GetMaterial(const char* name); // specify .ext
        Material* GetMaterialFromPath(const char* filePath);
        FT_Face GetFont(const char* name); // specify .ext
        FT_Face GetFontFromPath(const char* filePath);
        ALuint GetSound(const char* name); // specify .ext
        ALuint GetSoundFromPath(const char* filePath);
        ShaderProgram* GetShaderProgram(const char* name); // #TODO specify .ext
        ShaderProgram* GetShaderProgramFromPath(const char* filePath);
        ShaderComponent* GetShaderComponent(const char* name, eShaderComponentTypes componentType);
        ShaderComponent* GetShaderComponentFromPath(const char* filePath, eShaderComponentTypes componentType);

        const std::map<std::string, Mesh*>& SeeMeshes();
        const std::map<std::string, Texture*>& SeeTextures();
        const std::map<std::string, Material*>& SeeMaterials();
        const std::map<std::string, ALuint>& SeeSounds();
        const std::map<std::string, ShaderProgram*>& SeeShaderPrograms();
        const std::map<std::string, ShaderComponent*>& SeeShaderComponents();

        // #TODO Deprecate if possible and own asset creation inside of Assets
        void TextureLoaded(const char* name, int handle);
        bool TextureLoaded(const char* name, Texture* texture);
        bool MeshLoaded(const char* filePath, Mesh* mesh);
        bool MaterialLoaded(const char* name, Material* material);
        bool FontLoaded(const char* name, FT_Face font);
        bool SoundLoaded(const char* name, ALuint sound);
        bool ShaderProgramLoaded(ShaderProgram* shaderProgram);
        bool ShaderProgramLoaded(const char* name, ShaderProgram* ShaderProgram);
        bool ShaderComponentLoaded(const char* name, ShaderComponent* shaderComponent);
    }

}
