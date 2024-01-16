#include "QF_RenderRoutine.h"

#include "QC_StringHelpers.h"

#include "QC_Reflection.h"
#include "QF_Enums.h"
#include "QF_Log.h"
#include "QF_GraphicsHelpers.h"
#include "QF_Graphics_Header.h"
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

    void RenderRoutine::Initialize()
    {
        m_pParent->AddDrawRoutine(this);
        m_pRenderComp = (RenderComponent*)m_pParent->GetComponent(eComponentTags::Component_Render);
        m_Type = eRoutineTypes::Routine_Render;
        SetDrawFunctions();

        if (m_pRenderComp)
        {
            // Setup mesh, shader, material render time values for each renderable
            std::vector<Renderable>* renderables = m_pRenderComp->GetRenderableList();

            for (unsigned int i = 0; i < renderables->size(); i++)
            {
                if (renderables->at(i).GetShaderSchematic())
                {
                    // TODO: Handle null mesh, material, shader
                    renderables->at(i).GetMesh()->SetupShaderAttributes(renderables->at(i).GetShaderSchematic());
                }
            }

            if (m_pParent)
                ((RenderRoutine*)m_pParent->GetFirstDrawRoutineOfType(eRoutineTypes::Routine_Render))->ResetUniformList();
        }
        else
        {
            LOG_WARN("m_pRenderComp is nullptr for object {0}!", m_pParent->GetName().c_str());
        }
    }
    //// Private functions
    void RenderRoutine::DrawMeshData(const GameObject* a_Camera)
    {
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
            LOG_ERROR("{0} Camera component* {1} is null!", __FUNCTION__, VARNAME_TO_STR(t_pCamera));
            return;
        }

        std::vector<Renderable>* t_Renderables = m_pRenderComp->GetRenderableList();

        for (size_t i = 0; i < t_Renderables->size(); i++)
        {
            t_Renderables->at(i).GetShaderSchematic()->Use();

            for (unsigned int j = 0; j < m_UniformSetupList[i].size(); j++)
            {
                (this->*m_UniformSetupList[i][j])(t_pCamera, &t_Renderables->at(i));
            }

            t_Renderables->at(i).GetMesh()->Draw();
        }
    }

    void RenderRoutine::NullDraw(const GameObject* a_Camera)
    {
        // Look for valid models/materials
        if (m_pRenderComp)
        {
            // TODO: Do I need to check that other data is valid?
            m_DrawFunc = &RenderRoutine::DrawMeshData;
            SetDrawFunctions();
            if (m_DrawFunc == &RenderRoutine::DrawMeshData)
                DrawMeshData(a_Camera); // Draw current frame
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

        for (unsigned int i = 0; i < t_Renderables->size(); i++) // for each renderable
        {
            if (t_Renderables->at(i).SeeShaderSchematic()->SeeUniforms()->size() == 0) t_Renderables->at(i).GetShaderSchematic()->FindAttributesAndUniforms();
            const std::vector<std::string>* t_Uniforms = t_Renderables->at(i).SeeShaderSchematic()->SeeUniforms();
            Material* t_Material = t_Renderables->at(i).GetMaterialSchematic();

            /* Add functions to setup shader uniforms */
            for (size_t j = 0; j < t_Uniforms->size(); j++) // Setup uniforms
            {
                // Color
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
            /* More complex uniforms */
            // Material
            if (t_Material)
            {
                m_UniformSetupList[i].push_back(&RenderRoutine::SetupMaterialUniforms);
            }
            // Lighting
            if (t_Material) // TODO: Improve logic
            {
                m_UniformSetupList[i].push_back(&RenderRoutine::SetupLightingUniforms);
            }
        }

        CheckGraphicsErrors(__FILE__, __LINE__); // DEBUG:
    }

}
