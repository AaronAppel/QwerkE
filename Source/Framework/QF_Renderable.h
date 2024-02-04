#pragma once

#include <string>

#include "Libraries/glew/GL/glew.h"
#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Mesh.h"
#include "QF_Resources.h"

namespace QwerkE {

    class Mesh;
    class ShaderProgram;
    class Material;

    class Renderable
    {
    public:
        Renderable() = default;
        Renderable(ShaderProgram* shader, Material* material, Mesh* mesh)
        {
            m_Shader = shader;
            m_Material = material;
            m_Mesh = mesh;
        }

        void Initialize()
        {
            m_Shader = Resources::GetShaderProgram(m_ShaderName.c_str());
            m_Material = Resources::GetMaterial(m_MaterialName.c_str());
            m_Mesh = Resources::GetMesh(m_MeshName.c_str());
            // m_Mesh = Resources::GetMeshFromFile(m_MeshFileName.c_str());
        }

        const std::string& GetRenderableName() const { return m_RenderableName; }
        ShaderProgram* GetShader() { return m_Shader; }
        const ShaderProgram* SeeShader() const { return m_Shader; }
        Material* GetMaterial() { return m_Material; }
        Mesh* GetMesh() { return m_Mesh; }
        const Mesh* SeeMesh() const { return m_Mesh; }

        void SetRenderableName(std::string name) { m_RenderableName = name; }
        void SetShader(ShaderProgram* shader)
        {
            if (shader)
            {
                m_Shader = shader;
                if (m_Mesh)
                {
                    m_Mesh->SetupShaderAttributes(m_Shader);
                }
            }
        }

        void SetMaterial(Material* material)
        {
            if (material)
                m_Material = material;
        }

        void SetMesh(Mesh* mesh)
        {
            if (mesh)
            {
                m_Mesh = mesh;
                if (m_Shader)
                {
                    m_Mesh->SetupShaderAttributes(m_Shader);
                }
            }
        }

    private:
        MIRROR_PRIVATE_MEMBERS

        std::string m_RenderableName = gc_DefaultStringValue;
        ShaderProgram* m_Shader = nullptr;
        Material* m_Material = nullptr;
        Mesh* m_Mesh = nullptr;
        std::string m_ShaderName = gc_DefaultStringValue;
        std::string m_MaterialName = gc_DefaultStringValue;
        std::string m_MeshName = gc_DefaultStringValue;
        std::string m_MeshFileName = gc_DefaultStringValue;
    };

}
