#include "QF_RenderRoutine.h"

#include <string>
#include <vector>
#include <map>

#include "glm/glm/gtc/type_ptr.hpp"

#include "QF_CameraComponent.h"
#include "QF_Component.h"
#include "QF_Enums.h"
#include "QF_GameObject.h"
#include "QF_LightComponent.h"
#include "QF_Material.h"
#include "QF_Renderable.h"
#include "QF_RenderComponent.h"
#include "QF_Scene.h"
#include "QF_ShaderProgram.h"
#include "QF_ShaderVariable_Defines.h"
#include "QF_Texture.h"

namespace QwerkE {

    void RenderRoutine::Setup3DTransform(const ComponentCamera* camera, Renderable* renderable)
    {
        ShaderProgram* t_pShader = renderable->GetShader();

        mat4 worldMat(1.f);
        worldMat = glm::scale(worldMat, m_pParent->GetScale());
        const vec3& parentRotation = m_pParent->GetRotation();
        worldMat = glm::rotate(worldMat, glm::radians(parentRotation.x), camera->CamRight());
        worldMat = glm::rotate(worldMat, glm::radians(parentRotation.y), camera->CamUp());
        worldMat = glm::rotate(worldMat, glm::radians(parentRotation.z), camera->CamForward());
        worldMat = glm::translate(worldMat, m_pParent->GetPosition());
        t_pShader->SetUniformMat4(worldMatrix, glm::value_ptr(worldMat));

        t_pShader->SetUniformMat4(viewMatrix, glm::value_ptr(camera->GetViewMatrix()));

        t_pShader->SetUniformMat4(projectionMatrix, glm::value_ptr(camera->GetProjectionMatrix()));
    }

    void RenderRoutine::Setup2DTransform(const ComponentCamera* camera, Renderable* renderable)
    {
        ShaderProgram* t_pShader = renderable->GetShader();

        mat4 worldMat(1.f);
        worldMat = glm::scale(worldMat, m_pParent->GetScale());
        worldMat = glm::rotate(worldMat, 1.0f, m_pParent->GetRotation());
        worldMat = glm::translate(worldMat, m_pParent->GetPosition());
        t_pShader->SetUniformMat4(transform2D, glm::value_ptr(worldMat));
    }

    void RenderRoutine::SetupColorUniforms(const ComponentCamera* cameraObject, Renderable* renderable)
    {
        ShaderProgram* t_pShader = renderable->GetShader();
        vec4 t_Colour = vec4(0, 1, 0, 1); // m_pRenderComp->GetColour();

        t_pShader->SetUniformFloat4(objectColor, t_Colour.x, t_Colour.y, t_Colour.z, t_Colour.w);
    }

    void RenderRoutine::SetupMaterialUniforms(const ComponentCamera* a_Camera, Renderable* renderable)
    {
        Material* material = renderable->GetMaterial();
        const std::map<eMaterialMaps, Texture*>* materialList = material->SeeMaterials();
        ShaderProgram* shader = renderable->GetShader();
        // TODO: Check uniforms from : const std::vector<std::string>* uniforms = shader->SeeUniforms();

        /* Assign material texture values */
        int counter = 0;
        for (auto p : *materialList)
        {
            // TODO: Do not assign values the material does not use
            if (p.second == nullptr)
                continue;

            glActiveTexture(GL_TEXTURE0 + counter);
            glBindTexture(GL_TEXTURE_2D, p.second->s_Handle);

            // TODO: Stop activating textures the shader does not use
            // if (shader->SeeUniforms().contains(counter) == true)
            switch (p.first)
            {
            case eMaterialMaps::MatMap_Ambient:
                shader->SetUniformInt1(AmbientName, counter);
                break;
            case eMaterialMaps::MatMap_Diffuse:
                shader->SetUniformInt1(DiffuseName, counter);
                break;
            case eMaterialMaps::MatMap_Specular:
                shader->SetUniformInt1(SpecularName, counter);
                break;
            case eMaterialMaps::MatMap_Emissive:
                shader->SetUniformInt1(EmissiveName, counter);
                break;
            case eMaterialMaps::MatMap_Height:
                shader->SetUniformInt1(HeightName, counter);
                break;
            case eMaterialMaps::MatMap_Normals:
                shader->SetUniformInt1(NormalsName, counter);
                break;
            case eMaterialMaps::MatMap_Shininess:
                shader->SetUniformInt1(ShininessName, counter);
                break;
            case eMaterialMaps::MatMap_Opacity:
                shader->SetUniformInt1(OpacityName, counter);
                break;
            case eMaterialMaps::MatMap_Displacement:
                shader->SetUniformInt1(DisplacementName, counter);
                break;
            case eMaterialMaps::MatMap_LightMap:
                shader->SetUniformInt1(LightMapName, counter);
                break;
            case eMaterialMaps::MatMap_Reflection:
                shader->SetUniformInt1(ReflectionName, counter);
                break;
            case eMaterialMaps::MatMap_Albedo:
                shader->SetUniformInt1(AlbedoName, counter);
                break;
            case eMaterialMaps::MatMap_Metallic:
                shader->SetUniformInt1(MetallicName, counter);
                break;
            case eMaterialMaps::MatMap_AmbientOcclusion:
                shader->SetUniformInt1(AmbientOcclusionName, counter);
                break;
            case eMaterialMaps::MatMap_Roughness:
                shader->SetUniformInt1(RougnessName, counter);
                break;
            }
            counter++;
        }

        /* Assign other values */
        shader->SetUniformFloat1("Shine", 0.5f); // TODO: Improve
    }

    void RenderRoutine::SetupLightingUniforms(const ComponentCamera* a_Camera, Renderable* renderable)
    {
        ShaderProgram* t_pShader = renderable->GetShader();
        // TODO: Get light data better
        GameObject* t_Light = m_pRenderComp->GetParent()->GetScene()->GetLightList().at(0); // TODO: Stop hard coding index, plus handle multiple lights
        LightComponent* t_LightComp = (LightComponent*)t_Light->GetComponent(Component_Light);

        vec3 lightColour = t_LightComp->GetColour();

        vec3 t_LightPos = t_Light->GetPosition();
        t_pShader->SetUniformFloat3(lightPosition, t_LightPos.x, t_LightPos.y, t_LightPos.z);

        t_pShader->SetUniformFloat3(lightColor, lightColour.x, lightColour.y, lightColour.z);
    }

    void RenderRoutine::SetupCameraUniforms(const ComponentCamera* a_Camera, Renderable* renderable)
    {
        ShaderProgram* t_pShader = renderable->GetShader();

        vec3 t_CamPos = a_Camera->SeeParent()->GetPosition();
        t_pShader->SetUniformFloat3(cameraPosition, t_CamPos.x, t_CamPos.y, t_CamPos.z);
    }

    void RenderRoutine::SetupTextureUniforms(GLuint textures[], int numTextures, ShaderProgram* shader)
    {
        for (int i = 0; i < numTextures; i++)
        {
            std::string id = TexturePrefix;
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textures[i]);
            shader->SetUniformInt1(id.append(std::to_string(i)).c_str(), i);
        }
    }

}
