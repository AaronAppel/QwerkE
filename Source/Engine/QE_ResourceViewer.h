#pragma once

#include <string>
#include <map>
#include <vector>

#include "Libraries/glew/GL/glew.h"
#include "Libraries/imgui/QC_imgui.h"

#include "QF_Constants.h"

typedef unsigned int ALuint; // #TODO Hide OAL library type

namespace QwerkE {

    class ShaderProgram;
    class Scene;
    class GameObject;
    class Mesh;
    class Material;
    class MaterialEditor;

    struct Texture;

    class ResourceViewer // #TODO Consider for static namespace or singleton
    {
    public:
        ResourceViewer();
        ~ResourceViewer();

        void Draw();

    private:
        void RenderModelThumbnails();

        int m_CurrentResource = 0;

        MaterialEditor* m_MaterialEditor = nullptr;
        bool m_ShowMatEditor = false;
        std::string m_MatName = gc_DefaultStringValue;

        const std::map<std::string, ShaderProgram*>* m_Shaders = nullptr;
        const std::map<std::string, Material*>* m_Materials = nullptr;
        const std::map<std::string, Texture*>* m_Textures = nullptr;
        const std::map<std::string, Mesh*>* m_Meshes = nullptr;
        const std::map<std::string, ALuint>* m_Sounds = nullptr;

        unsigned char m_ModelsPerRow = 4;
        ImVec2 m_ModelThumbnailPixelSize = ImVec2(64, 64);

        std::vector<GLuint> m_ModelImageHandles;

        GameObject* m_ModelToDraw = nullptr;
        Scene* m_ViewerScene = nullptr;
    };

}
