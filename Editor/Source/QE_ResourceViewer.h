#pragma once

#include <string>
#include <map>
#include <vector>

#ifdef _QGLEW
#include "Libraries/glew/glew.h"
#endif

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#include "QF_Constants.h"

#include "QE_MaterialEditor.h"

#ifdef _QOPENAL
typedef unsigned int ALuint; // #TODO Hide OAL library type
#endif

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

        int m_CurrentSelectedResourceType = 0;

        uPtr<MaterialEditor> m_MaterialEditor;
        bool m_ShowMatEditor = false;
        std::string m_MatName = Constants::gc_DefaultStringValue;

        unsigned char m_ModelsPerRow = 4; // #TODO const and move out of header

#ifdef _QDEARIMGUI
        ImVec2 m_ModelThumbnailPixelSize = ImVec2(64, 64); // #TODO const and move out of header
#else
        vec2 m_ModelThumbnailPixelSize = vec2(64, 64); // #TODO const and move out of header
#endif

        std::vector<GLuint> m_ModelImageHandles; // #TODO Should not be in an instance as only 1 should exist

        GameObject* m_ModelToDraw = nullptr;
        Scene* m_ViewerScene = nullptr;
    };

}
