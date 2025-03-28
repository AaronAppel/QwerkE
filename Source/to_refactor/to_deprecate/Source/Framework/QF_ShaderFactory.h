#pragma once

#include <stdio.h>
#include <string>

#include "QF_Graphics_Header.h"
#include "QF_GraphicsHelpers.h"

namespace QwerkE {

    class ShaderProgram;
    class ShaderComponent;

    // TODO: Think of separating generated shader code somehow
    namespace ShaderFactory {

        ShaderProgram* CreateShader(eShaderTypes type);
        ShaderProgram* CreateShader(const char* vertFileDir, const char* fragFileDir, const char* geoFileDir);
        ShaderComponent* CreateShaderComponent(GLenum type, const char* filePath);
        ShaderComponent* GenerateShaderFromData(GLenum type, const char* shaderData);
        GLuint CreateShaderProgram(GLuint vert, GLuint frag, GLuint geo);
        // TODO: ShaderProgram* CreateShaderProgram(GLuint vert, GLuint frag, GLuint geo);

        bool LinkCreatedShaderProgram(ShaderProgram* shader);

        // TODO: Abstract library types
        GLuint CreateVertexShader(const char* vertPath);
        GLuint CreateFragmentShader(const char* fragPath);
        GLuint CreateGeometryShader(const char* geoPath);

        const char* CreateVertexShader(eShaderTypes shaderName);
        const char* CreateFragmentShader(eShaderTypes shaderName);
        const char* CreateGeometryShader(eShaderTypes shaderName);

        void SetShaderFileName(eShaderTypes shaderType, std::string& shaderString, std::string extension);
        void SetVertName(eShaderTypes shaderType, std::string& shaderString);
        void SetFragName(eShaderTypes shaderType, std::string& shaderString);
        void SetGeoName(eShaderTypes shaderType, std::string& shaderString);
        void SetRenderEngineVersion(std::string& shaderString);

        void OpenMain(std::string& shaderString);
        void CloseMain(std::string& shaderString);

        const char* GetAttributeNamePrefix();
        const char* GetUniformPrefix();
        const char* GetTransferPrefix();

        /* Setters */
        // void SetPrefixes(const char* attr, const char* unif, const char* trans);

        // TODO: Hide
        // INTERNAL

        // Linking
        GLuint LinkShaders(const ShaderComponent* vert, const ShaderComponent* frag, const ShaderComponent* geo);
        GLuint LinkShaders(GLuint vert, GLuint frag, GLuint geo);

        /* Write Shader functions */
        // Basic3D
        std::string Basic3DShaderVert(std::string vertString);
        std::string Basic3DShaderFrag(std::string fragString);
        // Basic2D
        std::string Basic2DShaderVert(std::string vertString);
        std::string Basic2DShaderFrag(std::string fragString);
        // LitMaterial
        std::string LitMaterialVert(std::string vertString);
        std::string LitMaterialFrag(std::string fragString);

        /* Add Shader Variables */
        // Structs
        void AddMaterialStruct(std::string& string);
        void AddUniformMaterialStruct(const char* name, std::string& string);

        void AddLightStruct(std::string& string);

        // Attributes
        // TODO: Try to reuse the following functions to reduce duplicating code
        void AddAttributeInt(const char* name, std::string& string);

        void AddAttributeFloat(const char* name, std::string& string);
        void AddAttributeVec2(const char* name, std::string& string);
        void AddAttributeVec3(const char* name, std::string& string);
        void AddAttributeVec4(const char* name, std::string& string);

        void AddAttributeMat2(const char* name, std::string& string);
        void AddAttributeMat3(const char* name, std::string& string);
        void AddAttributeMat4(const char* name, std::string& string);

        // Uniforms
        void AddUniformInt(const char* name, std::string& string);

        void AddUniformFloat(const char* name, std::string& string);
        void AddUniformVec2(const char* name, std::string& string);
        void AddUniformVec3(const char* name, std::string& string);
        void AddUniformVec4(const char* name, std::string& string);

        void AddUniformMat2(const char* name, std::string& string);
        void AddUniformMat3(const char* name, std::string& string);
        void AddUniformMat4(const char* name, std::string& string);

        void AddUniformSampler2D(const char* name, std::string string);

        void AddUniformTexture(int* count, std::string string);

        // Input
        void AddInputInt(const char* name, std::string& string);

        void AddInputFloat(const char* name, std::string& string);
        void AddInputVec2(const char* name, std::string& string);
        void AddInputVec3(const char* name, std::string& string);
        void AddInputVec4(const char* name, std::string& string);

        void AddInputMat2(const char* name, std::string& string);
        void AddInputMat3(const char* name, std::string& string);
        void AddInputMat4(const char* name, std::string& string);

        // Output
        void AddOutputInt(const char* name, std::string& string);

        void AddOutputFloat(const char* name, std::string& string);
        void AddOutputVec2(const char* name, std::string& string);
        void AddOutputVec3(const char* name, std::string& string);
        void AddOutputVec4(const char* name, std::string& string);

        void AddOutputMat2(const char* name, std::string& string);
        void AddOutputMat3(const char* name, std::string& string);
        void AddOutputMat4(const char* name, std::string& string);

        // #TODO void AddComplexAttribute(const char* name, std::string& string);

        template <typename T>
        char* CreatePrimitiveAttribute(const char* name) // Could replace AddAttributeInt() and similar functions
        {
            if (!name)
                return nullptr;

            // Ex. "\nin int a_name;"
            const char* formatString = "\n%s %s %s%s;";

            auto tpyeInfo = Mirror::InfoForType<T>();
            const char* typeName = tpyeInfo->stringName.c_str();
            const unsigned int bufferSize = 1 + strlen(InputKeyword) + 1 + strlen(typeName) + 1 + strlen(AttributeNamePrefix) + strlen(name) + 1 + 1;

            char* buffer = new char[bufferSize];
            buffer[bufferSize - 1] = '\0';
            snprintf(buffer, bufferSize, formatString, InputKeyword, Mirror::InfoForType<T>()->stringName.c_str(), AttributeNamePrefix, name);
            return buffer;
        }
    }

}
