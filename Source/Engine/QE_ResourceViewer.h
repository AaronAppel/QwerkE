#pragma once

#include <string>
#include <map>
#include <vector>

#include "Libraries/glew/GL/glew.h"
#include "Libraries/imgui/QC_imgui.h"

#include "QF_Constants.h"

#include "QE_MaterialEditor.h"

typedef unsigned int ALuint; // #TODO Hide OAL library type

namespace QwerkE {

    class ShaderProgram;
    class Scene;
    class GameObject;
    class Mesh;
    class Material;

    struct Texture;

    class ResourceViewer
    {
    public:
        ResourceViewer();
        ~ResourceViewer();

        void Draw();

    private:
        void RenderModelThumbnails();

        int m_CurrentResource = 0;

        uPtr<MaterialEditor> m_MaterialEditor;
        bool m_ShowMatEditor = false;
        std::string m_MatName = gc_DefaultStringValue;

        unsigned char m_ModelsPerRow = 4;
        ImVec2 m_ModelThumbnailPixelSize = ImVec2(64, 64);

        std::vector<GLuint> m_ModelImageHandles;

        GameObject* m_ModelToDraw = nullptr;
        Scene* m_ViewerScene = nullptr;
    };

}
