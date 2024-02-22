#include "QF_AssimpLoading.h"

#pragma warning( disable : 26495 )
#include "Libraries/assimp/Importer.hpp"
#include "Libraries/assimp/scene.h"
#include "Libraries/assimp/postprocess.h"
#include "Libraries/assimp/config.h"
#include "Libraries/assimp/material.h"
#pragma warning( default : 26495 )

#include "QC_StringHelpers.h"

#include "QF_Defines.h"
#include "QF_FileSystem.h"
#include "QF_FileUtilities.h"
#include "QF_GraphicsHelpers.h"
#include "QF_Material.h"
#include "QF_Mesh.h"
#include "QF_MeshData.h"
#include "QF_Assets.h"
#include "QF_Texture.h"

// TODO: Support triangle and quad rendering? Set a macro to define the number of verts per face?
// TODO: Split functions into smaller pieces. Ex. LoadMeshFromAI() can call functions to GetVertsFromAIMesh, GetUVs, GetMats... etc

namespace QwerkE {

    Mesh* _assimp_loadVertexData(aiMesh* mesh, const aiScene* scene, const char* modelFilePath);
    void _assimp_loadMaterialTextures(aiMaterial* mat, const std::string& filePath, std::vector<std::string>& matNames);

    void _assimp_loadSceneNodeData(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes, const std::string& filePath, std::vector<std::string>& matNames)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(_assimp_loadVertexData(mesh, scene, filePath.c_str()));

            if (mesh->mMaterialIndex >= 0)
            {
                _assimp_loadMaterialTextures(scene->mMaterials[mesh->mMaterialIndex], filePath, matNames);
            }
        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            _assimp_loadSceneNodeData(node->mChildren[i], scene, meshes, filePath, matNames);
        }
    }

    Mesh* _assimp_loadVertexData(aiMesh* mesh, const aiScene* scene, const char* modelFilePath)
    {
        char* modelFullFileName = File::FullFileName(modelFilePath);
        if (!modelFullFileName)
            return nullptr;

        if (Assets::MeshExists(modelFullFileName))
        {
            return Assets::GetMesh(modelFullFileName);
        }

        MeshData meshData;
        meshData.positions.resize(mesh->mNumVertices);
        meshData.UVs.resize(mesh->mNumVertices);
        meshData.normals.resize(mesh->mNumVertices);
        meshData.tangents.resize(mesh->mNumVertices);
        meshData.bitangents.resize(mesh->mNumVertices);

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            vec3 position;
            position.x = mesh->mVertices[i].x;
            position.y = mesh->mVertices[i].y;
            position.z = mesh->mVertices[i].z;

            meshData.positions[i] = position;

            // color
            // TODO: mesh->HasVertexColors()
            // vertex.color = vec4(mesh->mColors[i]->r, mesh->mColors[i]->g, mesh->mColors[i]->b, mesh->mColors[i]->a);

            meshData.UVs[i] = vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

            if (mesh->HasNormals())
            {
                meshData.normals[i] = vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            }

            if (mesh->HasTangentsAndBitangents())
            {
                meshData.tangents[i] = vec3(mesh->mTangents->x, mesh->mTangents->y, mesh->mTangents->z);
                meshData.bitangents[i] = vec3(mesh->mBitangents->x, mesh->mBitangents->y, mesh->mBitangents->z);
            }
        }

        std::vector<unsigned int> indices(mesh->mNumFaces * 3); // TODO: Swap to array? // PERF: really good use of a 1 frame stack allocation
        aiFace face;
        int counter = 0;

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices[counter++] = face.mIndices[j]; // For each vertex in face
            }
        }

        meshData.indices = indices;
        Mesh* rMesh = nullptr;

        if (meshData.positions.size() > 0)
        {
            rMesh = new Mesh();
            rMesh->BufferMeshData(&meshData);
            rMesh->SetName(mesh->mName.C_Str());
            rMesh->SetFileName(modelFullFileName);
            Assets::MeshLoaded(modelFullFileName, rMesh);
        }
        else
        {
            rMesh = Assets::GetMesh(null_mesh);
        }

        free(modelFullFileName);
        return rMesh;
    }

    void _assimp_loadMaterialTextures(aiMaterial* mat, const std::string& filePath, std::vector<std::string>& matNames)
    {
        // check if material is already loaded
        aiString name;
        mat->Get(AI_MATKEY_NAME, name);
        // TODO: Support "DefaultMaterial"
        if (Assets::MaterialExists(name.C_Str()))
        {
            // material exists already
        }
        else
        {
            Material* material = new Material(); // TODO: delete if null
            material->SetMaterialName(name.C_Str());
            // load each map/texture into a new material

            // iterate through all known texture types, then load the textures
            // into the Resources and assign handles to the return material.
            // mat->GetTextureCount(); // optimization

            int num = aiTextureType_UNKNOWN - 1;
            for (int i = aiTextureType_NONE + 1; i < num; i++)
            {
                aiString str;
                Texture* texture = nullptr;

                mat->GetTexture((aiTextureType)i, 0, &str);

                if (*str.C_Str())
                {
                    texture = Assets::GetTexture(str.C_Str());
                }

                switch (i)
                {
                case aiTextureType_DIFFUSE:
                    material->AddTexture(texture, eMaterialMaps::MatMap_Diffuse);
                    break;
                case aiTextureType_SPECULAR:
                    material->AddTexture(texture, eMaterialMaps::MatMap_Specular);
                    break;
                case aiTextureType_AMBIENT:
                    material->AddTexture(texture, eMaterialMaps::MatMap_Ambient);
                    break;
                case aiTextureType_EMISSIVE:
                    material->AddTexture(texture, eMaterialMaps::MatMap_Emissive);
                    break;
                case aiTextureType_HEIGHT:
                    material->AddTexture(texture, eMaterialMaps::MatMap_Height);
                    break;
                case aiTextureType_NORMALS:
                    material->AddTexture(texture, eMaterialMaps::MatMap_Normals);
                    break;
                case aiTextureType_SHININESS:
                    material->AddTexture(texture, eMaterialMaps::MatMap_Shininess);
                    break;
                case aiTextureType_OPACITY:
                    material->AddTexture(texture, eMaterialMaps::MatMap_Opacity);
                    break;
                case aiTextureType_DISPLACEMENT:
                    material->AddTexture(texture, eMaterialMaps::MatMap_Displacement);
                    break;
                case aiTextureType_LIGHTMAP:
                    material->AddTexture(texture, eMaterialMaps::MatMap_LightMap);
                    break;
                case aiTextureType_REFLECTION:
                    material->AddTexture(texture, eMaterialMaps::MatMap_Reflection);
                    break;
                }
            }

            Assets::MaterialLoaded(name.C_Str(), material);
        }

        matNames.push_back(name.C_Str()); // #NOTE Material loader will use this name later
    }

    void _assimp_loadMeshByName(aiNode* node, const aiScene* scene, Mesh*& mesh, const char* modeFilePath, const char* meshName)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
            if (strcmp(aimesh->mName.C_Str(), meshName) == 0)
            {
                mesh = _assimp_loadVertexData(aimesh, scene, modeFilePath);
                return;
            }
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            _assimp_loadMeshByName(node->mChildren[i], scene, mesh, modeFilePath, meshName);
        }
    }

}
