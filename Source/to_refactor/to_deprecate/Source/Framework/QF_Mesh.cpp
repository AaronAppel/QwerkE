#include "QF_Mesh.h"

#include <assert.h>
#include <string>
#include <vector>

#include "Libraries/glew/glew.h"

#include "QC_StringHelpers.h"

#include "QF_Log.h"

#include "QF_GraphicsHelpers.h"
#include "QF_MeshData.h"
#include "QF_ShaderFactory.h"
#include "QF_ShaderProgram.h"
#include "QF_ShaderVariable_Defines.h"

namespace QwerkE {

    // #TODO Review comments

    Mesh::Mesh()
    {
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO); // #TODO Always generate an EBO? What if a mesh does not use indices (silly, but still)?
        glGenVertexArrays(1, &m_VAO);
    }

    Mesh::~Mesh()
    {
        // #TODO Move code to a function (Destroy() or something) to be used for unloading mesh data and re-buffering
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
        glDeleteVertexArrays(1, &m_VAO);
    }

    void Mesh::BufferMeshData(MeshData* data)
    {
        // #TODO Return true or false to communicate successful buffering?
        if (!data)
        {
            LOG_ERROR("MeshData is null!");
            assert(false);
        }

        // #TODO If VBO has data in it, handle deletion and re-assignment of new data
        if (m_BufferData.numPositions != 0) // Empty mesh?
        {
            LOG_ERROR("Mesh already has vertex data!");
            assert(false);
        }

        m_BufferData = data->BufferInfo();

        if (m_BufferData.BufferSize() < 1)
        {
            LOG_ERROR("Error buffering mesh vertex data!");
            assert(false);
        }

        // Pack model data into a contiguous buffer
        char* buffer = PackModelData(data);

        // Buffer mesh data into GPU RAM
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_BufferData.BufferSize(), buffer, GL_STATIC_DRAW); // (target, size, data, static/dynamic)

        if (m_BufferData.numIndices > 0)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_BufferData.numIndices, data->indices.data(), GL_STATIC_DRAW);
        }

        // Unbind for safety
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        CheckGraphicsErrors(__FILE__, __LINE__);

        delete[] buffer;
    }

    char* Mesh::PackModelData(MeshData* data)
    {
        char* buffer = new char[m_BufferData.BufferSize()];
        int currentIndex = 0;

        // Write raw data into a buffer, but only if it was loaded into the MeshData object
        memcpy(&buffer[currentIndex], data->positions.data(), data->positions.size() * sizeof(vec3));
        currentIndex += data->positions.size() * sizeof(vec3);
        memcpy(&buffer[currentIndex], data->colors.data(), data->colors.size() * sizeof(vec4));
        currentIndex += data->colors.size() * sizeof(vec4);
        memcpy(&buffer[currentIndex], data->UVs.data(), data->UVs.size() * sizeof(vec2));
        currentIndex += data->UVs.size() * sizeof(vec2);
        memcpy(&buffer[currentIndex], data->normals.data(), data->normals.size() * sizeof(vec3));
        currentIndex += data->normals.size() * sizeof(vec3);
        memcpy(&buffer[currentIndex], data->tangents.data(), data->tangents.size() * sizeof(vec3));
        currentIndex += data->tangents.size() * sizeof(vec3);
        memcpy(&buffer[currentIndex], data->bitangents.data(), data->bitangents.size() * sizeof(vec3));
        // currentIndex += data->bitangents.size() * sizeof(vec3);

        return buffer;
    }

    void Mesh::SetupShaderAttributes(ShaderProgram* shader) // #TODO Review const shader*
    {
        // Tell the shader where to look for the mesh data.
        // This needs to be called on shader changes but does not
        // affect the mesh buffered data at all.

        CheckGraphicsErrors(__FILE__, __LINE__);

        if (shader == nullptr)
            return;

        if (m_BufferData.numIndices > 0)
        {
            m_DrawFunc = &Mesh::DrawElements;
        }
        else if (m_BufferData.numPositions > 0)
        {
            m_DrawFunc = &Mesh::DrawArrays;
        }
        else
        {
            LOG_WARN("Mesh assigned null draw method");
            m_DrawFunc = &Mesh::NullDraw;
        }

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

        CheckGraphicsErrors(__FILE__, __LINE__);

        // Assign ShaderProgram() attribute data by initializing m_VAO attribute values
        const std::vector<std::string>* attributes = shader->SeeAttributes();
        int vertexDataStride = 0; // NOTE: Currently values are packed contiguously. No values are skipped when reading.

        for (unsigned int i = 0; i < attributes->size(); i++)
        {
            std::string attributeName = DispStrCombine(ShaderFactory::GetAttributeNamePrefix(), attributes->at(i).c_str());
            GLuint attributeLoc = glGetAttribLocation(shader->GetProgram(), attributeName.c_str());

            if (attributeLoc != -1)
            {
                // #NOTE glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer)

                if (StringCompare(attributes->at(i), vertexPosition))
                    glVertexAttribPointer(attributeLoc, 3, GL_FLOAT, GL_FALSE, vertexDataStride, (GLvoid*)m_BufferData.PositionOffset());

                else if (StringCompare(attributes->at(i), vertexColour))
                    glVertexAttribPointer(attributeLoc, 4, GL_FLOAT, GL_FALSE, vertexDataStride, (GLvoid*)m_BufferData.ColorOffset());

                else if (StringCompare(attributes->at(i), vertexUV))
                    glVertexAttribPointer(attributeLoc, 2, GL_FLOAT, GL_FALSE, vertexDataStride, (GLvoid*)m_BufferData.UVOffset());

                else if (StringCompare(attributes->at(i), vertexNormal))
                    glVertexAttribPointer(attributeLoc, 3, GL_FLOAT, GL_FALSE, vertexDataStride, (GLvoid*)m_BufferData.NormalOffset());

                else if (StringCompare(attributes->at(i), vertexTangent))
                    glVertexAttribPointer(attributeLoc, 3, GL_FLOAT, GL_FALSE, vertexDataStride, (GLvoid*)m_BufferData.TangentsOffset());

                else if (StringCompare(attributes->at(i), vertexBitangent))
                    glVertexAttribPointer(attributeLoc, 3, GL_FLOAT, GL_FALSE, vertexDataStride, (GLvoid*)m_BufferData.BitangentsOffset());
                else
                {
                    LOG_WARN("{0} Attribute name not found/supported: {1}", __FUNCTION__, attributes->at(i).c_str());
                    continue;
                }

                glEnableVertexAttribArray(attributeLoc);
            }
            else
            {
                LOG_WARN("Attribute name not found/supported: {0}", attributes->at(i).c_str());
            }
        }
        // #TODO Check if vertex arrays should to be disabled after drawing
        glBindVertexArray(0);
        CheckGraphicsErrors(__FILE__, __LINE__);
    }

    void Mesh::DrawElements() const
    {
        glBindVertexArray(m_VAO);
        // #NOTE If there is a read access violation, it is because SetupAttributes was not called
        glDrawElements(m_PrimitiveType, m_BufferData.numIndices, GL_UNSIGNED_INT, 0); // (drawMode, numIndices, EBOType, dataOffset)
        glBindVertexArray(0);
    }

    void Mesh::DrawArrays() const
    {
        glBindVertexArray(m_VAO);
        glDrawArrays(m_PrimitiveType, 0, m_BufferData.numPositions); // (drawMode, firstIndex, numVerts)
        glBindVertexArray(0);
    }

    void Mesh::NullDraw() const
    {
        // #TODO Review possible re-attempt at setup or removal of method body
        // SetupShaderAttributes();
        // if (m_IndexCount > 0) { DrawElements(); }
        // else if (m_VertCount > 0) { DrawArrays(); }
    }

    void Mesh::ToggleWireframe()
    {
        if (m_PrimitiveType == GL_TRIANGLES) // #TODO Examine temporary hack
        {
            m_PrimitiveType = GL_POINTS;
        }
        else
        {
            m_PrimitiveType = GL_TRIANGLES;
        }
    }

}
