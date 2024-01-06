#pragma once

#include "Libraries/glew/GL/glew.h"

#include "QF_Mesh.h"

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

        // Getters + Setters
        // Getters
        std::string GetRenderableName() const { return m_RenderableName; }
        ShaderProgram* GetShaderSchematic() { return m_Shader; }
        Material* GetMaterialSchematic() { return m_Material; }
        Mesh* GetMesh() { return m_Mesh; }

        // Setters
        void SetRenderableName(std::string name) { m_RenderableName = name; }
        void SetShader(ShaderProgram* shader)
        {
            if (shader)
            {
                m_Shader = shader;
                if (m_Mesh)
                    m_Mesh->SetupShaderAttributes(m_Shader);
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
        std::string m_RenderableName = gc_DefaultStringValue;
        ShaderProgram* m_Shader = nullptr;
        Material* m_Material = nullptr;
        Mesh* m_Mesh = nullptr;
    };

}