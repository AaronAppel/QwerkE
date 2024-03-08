#include "QF_RenderRoutine.h"

#include "Libraries/Mirror/Source/Mirror.h"

#include "QC_StringHelpers.h"

#include "QF_Enums.h"
#include "QF_Log.h"
#include "QF_GraphicsHelpers.h"
#include "QF_GraphicsHeader.h"
#include "QF_Material.h"
#include "QF_Renderable.h"
#include "QF_ShaderProgram.h"
#include "QF_Mesh.h"
#include "QF_Material.h"
#include "QF_GameObject.h"
#include "QF_Routine.h"
#include "QF_CameraComponent.h"
#include "QF_RenderComponent.h"

namespace QwerkE {

    RenderRoutine::RenderRoutine()
    {
        m_Type = eRoutineTypes::Routine_Render;
    }

    void RenderRoutine::SetParent(GameObject* a_Parent)
    {
        if (m_pParent)
            return;

        m_pParent = a_Parent;
        Initialize();
    }

    void RenderRoutine::Initialize()
    {
        m_pParent->AddDrawRoutine(this);
        m_pRenderComp = (RenderComponent*)m_pParent->GetComponent(eComponentTags::Component_Render);
        m_Type = eRoutineTypes::Routine_Render;
        SetDrawFunctions();

        if (m_pRenderComp)
        {
            std::vector<Renderable>* renderables = m_pRenderComp->GetRenderableList();

            for (unsigned int i = 0; i < renderables->size(); i++)
            {
                if (renderables->at(i).GetShader())
                {
                    // TODO: Handle null mesh, material, shader
                    renderables->at(i).GetMesh()->SetupShaderAttributes(renderables->at(i).GetShader());
                }
            }

            if (m_pParent)
            {
                RenderRoutine* renderRoutine = (RenderRoutine*)m_pParent->GetFirstDrawRoutineOfType(eRoutineTypes::Routine_Render);
                renderRoutine->ResetUniformList();
            }
        }
        else
        {
            LOG_WARN("m_pRenderComp is nullptr for object {0}!", m_pParent->GetName().c_str());
        }
    }

    void RenderRoutine::DrawMeshData(const GameObject* a_Camera)
    {
        if (!m_Enabled)
            return;

        if (!m_pRenderComp)
        {
            LOG_WARN("m_pRenderComp is nullptr for object {0}!", m_pParent->GetName().c_str());
            m_DrawFunc = &RenderRoutine::NullDraw;
            return;
        }

        auto a = a_Camera->SeeComponents();
        const ComponentCamera* t_pCamera = nullptr;
        if (a->find(Component_Camera) != a->end())
        {
            t_pCamera = (ComponentCamera*)a->find(Component_Camera)->second;
        }

        if (!t_pCamera)
        {
            LOG_ERROR("{0} Camera component* {1} is null!", __FUNCTION__, MIRROR_TO_STR(t_pCamera));
            return;
        }

        std::vector<Renderable>* t_Renderables = m_pRenderComp->GetRenderableList();

        for (size_t i = 0; i < t_Renderables->size(); i++)
        {
            t_Renderables->at(i).GetShader()->Use();

            for (unsigned int j = 0; j < m_UniformSetupList[i].size(); j++)
            {
                (this->*m_UniformSetupList[i][j])(t_pCamera, &t_Renderables->at(i));
            }

            t_Renderables->at(i).GetMesh()->Draw();
        }
    }

    void RenderRoutine::NullDraw(const GameObject* a_Camera)
    {
        if (m_Enabled && m_pRenderComp)
        {
            m_DrawFunc = &RenderRoutine::DrawMeshData; // TODO: Do I need to check that other data is valid?
            SetDrawFunctions();

            if (m_DrawFunc == &RenderRoutine::DrawMeshData)
            {
                DrawMeshData(a_Camera);
            }
        }
    }

    void RenderRoutine::SetDrawFunctions()
    {
        if (!m_pRenderComp)
        {
            LOG_WARN("m_pRenderComp is nullptr for object {0}!", m_pParent->GetName().c_str());
            m_DrawFunc = &RenderRoutine::NullDraw;
            return;
        }

        m_UniformSetupList.clear(); // reset uniform func list

        std::vector<Renderable>* t_Renderables = (std::vector<Renderable>*)m_pRenderComp->LookAtRenderableList();

        // TODO:: Improve conditions for assignments.
        // append empty vectors to fill
        for (size_t i = 0; i < t_Renderables->size(); i++)
        {
            std::vector<SetupUniformFunction> temp;
            m_UniformSetupList.push_back(temp);
        }

        CheckGraphicsErrors(__FILE__, __LINE__);
        for (unsigned int i = 0; i < t_Renderables->size(); i++)
        {
            if (t_Renderables->at(i).SeeShader()->SeeUniforms()->size() == 0)
            {
                t_Renderables->at(i).GetShader()->FindAttributesAndUniforms();
            }
            const std::vector<std::string>* t_Uniforms = t_Renderables->at(i).SeeShader()->SeeUniforms();
            Material* t_Material = t_Renderables->at(i).GetMaterial();

            for (size_t j = 0; j < t_Uniforms->size(); j++)
            {
                if (t_Uniforms->at(j) == "ObjectColor")
                {
                    m_UniformSetupList[i].push_back(&RenderRoutine::SetupColorUniforms);
                }
                else if (t_Uniforms->at(j) == "WorldMat")
                {
                    m_UniformSetupList[i].push_back(&RenderRoutine::Setup3DTransform);
                }
                else if (t_Uniforms->at(j) == "2DTransform")
                {
                    m_UniformSetupList[i].push_back(&RenderRoutine::Setup2DTransform);
                }
            }

            if (t_Material)
            {
                m_UniformSetupList[i].push_back(&RenderRoutine::SetupMaterialUniforms);
            }

            if (t_Material)
            {
                m_UniformSetupList[i].push_back(&RenderRoutine::SetupLightingUniforms);
            }
        }

        CheckGraphicsErrors(__FILE__, __LINE__);
    }

}
