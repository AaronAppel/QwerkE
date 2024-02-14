#include "QF_RenderComponent.h"

#include "QF_Defines.h"
#include "QF_GameObject.h"
#include "QF_GraphicsHelpers.h"
#include "QF_Material.h"
#include "QF_RenderRoutine.h"
#include "QF_Resources.h"
#include "QF_ShaderProgram.h"

namespace QwerkE {

    RenderComponent::RenderComponent()
    {
        m_ComponentTag = eComponentTags::Component_Render;
    }

    RenderComponent::RenderComponent(const char* objectRecipe)
    {
        m_ComponentTag = eComponentTags::Component_Render;

        if (m_pParent)
        {
            if (RenderRoutine* rRoutine = (RenderRoutine*)m_pParent->GetFirstDrawRoutineOfType(eRoutineTypes::Routine_Render))
            {
                rRoutine->ResetUniformList();
            }
        }
    }

    RenderComponent::RenderComponent(const char* shaderName, const char* materialName, const char* meshName)
    {
        m_ComponentTag = eComponentTags::Component_Render;

        Renderable t_Renderable;
        t_Renderable.SetMaterial(Resources::GetMaterial(materialName));
        t_Renderable.SetMesh(Resources::GetMesh(meshName));
        t_Renderable.SetShader(Resources::GetShaderProgram(shaderName));

        t_Renderable.GetMesh()->SetupShaderAttributes(t_Renderable.GetShader());

        m_Renderables.push_back(t_Renderable);

        if (m_pParent)
        {
            if (RenderRoutine* rRoutine = (RenderRoutine*)m_pParent->GetFirstDrawRoutineOfType(eRoutineTypes::Routine_Render))
            {
                rRoutine->ResetUniformList();
            }
        }
    }

    void RenderComponent::Activate()
    {
        for (size_t i = 0; i < m_Renderables.size(); i++)
        {
            Renderable& renderable = m_Renderables[i];
            renderable.Initialize();
        }
    }

    void RenderComponent::Setup(const char* shaderName, const char* materialName, const char* meshName)
    {
        // #TODO Should there be a check to see if a renderable alrady exists?
        Renderable t_Renderable;

        t_Renderable.SetMaterial(Resources::GetMaterial(materialName));
        t_Renderable.SetMesh(Resources::GetMesh(meshName));
        t_Renderable.SetShader(Resources::GetShaderProgram(shaderName));

        t_Renderable.GetMesh()->SetupShaderAttributes(t_Renderable.GetShader());

        m_Renderables.push_back(t_Renderable);

        if (m_pParent)
        {
            if (RenderRoutine* rRoutine = (RenderRoutine*)m_pParent->GetFirstDrawRoutineOfType(eRoutineTypes::Routine_Render))
            {
                rRoutine->ResetUniformList();
            }
        }
    }

    void RenderComponent::AppendEmptyRenderables(int count)
    {
        for (int i = 0; i < count; i++)
        {
            Renderable t_Renderable;

            t_Renderable.SetShader(Resources::GetShaderProgram(null_shader_schematic));
            t_Renderable.SetMaterial(Resources::GetMaterial(null_material));
            t_Renderable.SetMesh(Resources::GetMesh(null_mesh));

            m_Renderables.push_back(t_Renderable);
        }
    }

    void RenderComponent::AddRenderable(Renderable renderable)
    {
        m_Renderables.push_back(renderable);
    }

    void RenderComponent::SetNameAtIndex(unsigned int index, std::string name)
    {
        // TODO: More error handling
        if (index < m_Renderables.size())
        {
            m_Renderables[index].SetRenderableName(name);
        }
    }

    void RenderComponent::SetShaderAtIndex(unsigned int index, ShaderProgram* shader)
    {
        // TODO: More error handling
        if (index < m_Renderables.size() && shader != nullptr)
        {
            m_Renderables[index].SetShader(shader);

            if (m_pParent)
            {
                if (RenderRoutine* rRoutine = (RenderRoutine*)m_pParent->GetFirstDrawRoutineOfType(eRoutineTypes::Routine_Render))
                {
                    rRoutine->ResetUniformList();
                }
            }
        }
    }

    void RenderComponent::SetMaterialAtIndex(unsigned int index, Material* material)
    {
        // TODO: More error handling
        if (index < m_Renderables.size() && material != nullptr)
        {
            m_Renderables[index].SetMaterial(material);
        }
        // TODO: Changing material will need to reset render routine in the future
    }

    void RenderComponent::SetMeshAtIndex(unsigned int index, Mesh* mesh)
    {
        // TODO: More error handling
        if (index < m_Renderables.size() && mesh != nullptr)
        {
            m_Renderables[index].SetMesh(mesh);

            if (m_Renderables[index].GetShader())
            {
                m_Renderables[index].GetMesh()->SetupShaderAttributes(m_Renderables[index].GetShader());

                if (m_pParent)
                {
                    if (RenderRoutine* rRoutine = (RenderRoutine*)m_pParent->GetFirstDrawRoutineOfType(eRoutineTypes::Routine_Render))
                    {
                        rRoutine->ResetUniformList();
                    }
                }
            }
        }
    }

}
