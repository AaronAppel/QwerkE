#pragma once

#include <vector>

#include "Libraries/glew/GL/glew.h"

#include "QF_Routine.h"
#include "QF_Renderable.h"

namespace QwerkE {

    class ComponentCamera;
    class GameObject;
    class Material;
    class Mesh;
    class RenderComponent;
    class ShaderProgram;

    class RenderRoutine : public Routine
    {
    public:
        typedef void (RenderRoutine::* DrawFunc)(const GameObject* a_Camera); // draw mesh or model
        // TODO: Look at improving arguments
        typedef void (RenderRoutine::* SetupUniformFunction)(const ComponentCamera* a_Camera, Renderable* renderable); // Setup shader values function

    public:
        RenderRoutine();

        void SetParent(GameObject* a_Parent) override;

        void Initialize();

        void Draw(GameObject* a_Camera) { (this->*m_DrawFunc)(a_Camera); };

        void ResetUniformList() { SetDrawFunctions(); }; // TODO: External use?

    private:
        /* Private variables */
        DrawFunc m_DrawFunc = &RenderRoutine::NullDraw;
        RenderComponent* m_pRenderComp = nullptr;
        std::vector<std::vector<SetupUniformFunction>> m_UniformSetupList;

        bool m_3D = true; // 2D/3D optimization TODO: Remove?

        ////* Private functions *////
        void DrawMeshData(const GameObject* a_Camera);
        void NullDraw(const GameObject* a_Camera); // not setup

        //// Uniform value assignment ////
        void SetDrawFunctions();

        /* Vertex uniform value assignment */
        void Setup3DTransform(const ComponentCamera* a_Camera, Renderable* renderable);
        void Setup2DTransform(const ComponentCamera* a_Camera, Renderable* renderable);

        /* Fragment uniform value assignment */
        void SetupColorUniforms(const ComponentCamera* a_Camera, Renderable* renderable);
        void SetupMaterialUniforms(const ComponentCamera* a_Camera, Renderable* renderable);

        // Lighting
        void SetupLightingUniforms(const ComponentCamera* a_Camera, Renderable* renderable);
        // Camera
        void SetupCameraUniforms(const ComponentCamera* a_Camera, Renderable* renderable);
        /* Other */
        void SetupTextureUniforms(GLuint textures[], int numTextures, ShaderProgram* shader);
    };

}
