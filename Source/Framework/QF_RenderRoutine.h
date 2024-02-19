#pragma once

#include <vector>

#include "Libraries/glew/GL/glew.h"

#include "QF_Routine.h"

namespace QwerkE {

    class ComponentCamera;
    class GameObject;
    class RenderComponent;
    class ShaderProgram;

    class RenderRoutine : public Routine
    {
    public:
        typedef void (RenderRoutine::* DrawFunc)(const GameObject* a_Camera);
        typedef void (RenderRoutine::* SetupUniformFunction)(const ComponentCamera* a_Camera, Renderable* renderable);

    public:
        RenderRoutine();

        void SetParent(GameObject* a_Parent) override;

        void Initialize();

        void Draw(GameObject* a_Camera) { (this->*m_DrawFunc)(a_Camera); };

        void ResetUniformList() { SetDrawFunctions(); };

    private:
        DrawFunc m_DrawFunc = &RenderRoutine::NullDraw;
        RenderComponent* m_pRenderComp = nullptr;
        std::vector<std::vector<SetupUniformFunction>> m_UniformSetupList;

        void DrawMeshData(const GameObject* a_Camera);
        void NullDraw(const GameObject* a_Camera);

        void SetDrawFunctions();

        void Setup3DTransform(const ComponentCamera* a_Camera, Renderable* renderable);
        void Setup2DTransform(const ComponentCamera* a_Camera, Renderable* renderable);

        void SetupColorUniforms(const ComponentCamera* a_Camera, Renderable* renderable);
        void SetupMaterialUniforms(const ComponentCamera* a_Camera, Renderable* renderable);
        void SetupLightingUniforms(const ComponentCamera* a_Camera, Renderable* renderable);
        void SetupCameraUniforms(const ComponentCamera* a_Camera, Renderable* renderable);
        void SetupTextureUniforms(GLuint textures[], int numTextures, ShaderProgram* shader);
    };

}
