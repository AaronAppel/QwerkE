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
    class FrameBufferObject;
    class Mesh;
    class Material;
    class MaterialEditor;

    struct Texture;

    class ResourceViewer // #TODO Rename class to imgui_ResourceViewer
    {
    public:
        ResourceViewer();
        ~ResourceViewer();

        void Draw();

    private:
        int m_CurrentResource = 0; // #TODO Consider keeping an index for all windows for back tracking

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

        FrameBufferObject* m_FBO;
        std::vector<GLuint> m_ModelImageHandles;
        void DrawModelThumbnails();

        GameObject* m_ModelToDraw = nullptr;
        GameObject* m_AssetTagPlane = nullptr;
        Scene* m_ViewerScene = nullptr;
    };

}
