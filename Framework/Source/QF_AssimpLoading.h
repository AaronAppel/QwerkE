#pragma once

#include <iostream>
#include <vector>
#include <string>

struct aiNode;
struct aiScene;

namespace QwerkE {

    class Mesh;

    // TODO: Load model as 1 mesh
    void _assimp_loadSceneNodeData(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes, const std::string& filePath, std::vector<std::string>& matNames);
    void _assimp_loadMeshByName(aiNode* node, const aiScene* scene, Mesh*& mesh, const char* modelFilePath, const char* meshName);
    void _assimp_loadModelAs1Mesh(aiNode* node, const aiScene* scene, Mesh*& mesh, const char* modelFilePath);

}
