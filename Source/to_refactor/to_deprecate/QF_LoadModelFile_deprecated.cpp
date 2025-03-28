#include "QF_FileSystem.h"

#include <iostream>
#include <vector>
#include <string>

#pragma warning( disable : 26495 )
#include "Libraries/assimp/Importer.hpp"
#include "Libraries/assimp/scene.h"
#include "Libraries/assimp/postprocess.h"
#include "Libraries/assimp/config.h"
#include "Libraries/assimp/material.h"
#pragma warning( default : 26495 )

// define generic functions for model loading but
// use library specific prototype and body defines
#ifdef AI_CONFIG_H_INC // assimp
#include "QF_AssimpLoading.h"
#define TextureDirOffset(a, b) StringAppend("../Models/", a, b)
#else
#pragma error "Define model loading library!"
#endif

#include "QC_StringHelpers.h"

#include "QF_Resources.h"
#include "QF_Mesh.h"
#include "QF_FileUtilities.h"
#include "QF_Log.h"
#include "QF_Defines.h"

namespace QwerkE {

	namespace FileLoader {

		Mesh* LoadMeshInModelByName(const char* modelFilePath, const char* meshName)
		{
			if (false == Resources::MeshExists(meshName))
			{
				Mesh* mesh = nullptr;
#ifdef AI_CONFIG_H_INC // assimp
				Assimp::Importer importer;
				const aiScene *scene = importer.ReadFile(modelFilePath, aiProcess_Triangulate | aiProcess_FlipUVs); // TODO: Add aiProcess_CalcTangentSpace

				if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
				{
                    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
					LOG_ERROR("ERROR::ASSIMP::{0}", importer.GetErrorString());
					return nullptr; // failure
				}
				_assimp_loadMeshByName(scene->mRootNode, scene, mesh, modelFilePath, meshName);
#else
				// separate model loading library
#pragma error "Define model loading library!"
#endif // AI_CONFIG_H_INC
				Resources::AddMesh(meshName, mesh);
				return Resources::GetMesh(meshName);
			}
			else
			{
				return Resources::GetMesh(meshName);
			}
		}

		bool LoadModelFileToMeshes(const char* path)
		{
			// TODO: Check if a model with the same name already exists

			// TODO: Prevent the same model from being loaded

			// create temporary mesh and material buffers to load.
			// external library only cares about copying the data, not initializing it.
			std::vector<Mesh*> meshes;
			std::vector<std::string> matNames;

#ifdef AI_CONFIG_H_INC // assimp

			Assimp::Importer importer;
		    /*
			aiProcess_GenNormals: actually creates normals for each vertex if the model didn't contain normal vectors.
			aiProcess_SplitLargeMeshes : splits large meshes into smaller sub - meshes which is useful if your rendering has a maximum number of vertices allowed and can only process smaller meshes.
			aiProcess_OptimizeMeshes : actually does the reverse by trying to join several meshes into one larger mesh, reducing drawing calls for optimization.
			*/
			const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs); // TODO: Add aiProcess_CalcTangentSpace

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
                std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
                LOG_ERROR("ERROR::ASSIMP::{0}", importer.GetErrorString());
				return false; // failure
			}

			// NOTE: aiScene is an object hierarchy so it might almost require GameObject creation here.
			_assimp_loadSceneNodeData(scene->mRootNode, scene, meshes, path, matNames);

			// TODO: De-allocate RAM created by assimp
#else
// separate model loading library
#pragma error "Define model loading library!"
#endif // AI_CONFIG_H_INC

			// take copied data from external library and init it for QwerkE systems use.
			for (size_t i = 0; i < meshes.size(); i++)
			{
				Resources::AddMesh(meshes[i]->GetName().c_str(), meshes[i]);
			}
			for (size_t i = 0; i < matNames.size(); i++)
			{
				// TODO: Do something with the .mtl file loaded material names?
				// resMan->GetTexture(DispStrCombine(directory.c_str(), matNames[i].c_str()).c_str());
			}

			return true; // success
		}

		Mesh* LoadModelFileTo1Mesh(const char* modelFilePath)
		{
			if (false == Resources::MeshExists(GetFileNameNoExt(modelFilePath).c_str()))
			{
				Mesh* mesh = nullptr;

#ifdef AI_CONFIG_H_INC // assimp
				Assimp::Importer importer;
				const aiScene *scene = importer.ReadFile(modelFilePath, aiProcess_Triangulate | aiProcess_FlipUVs); // TODO: Add aiProcess_CalcTangentSpace

				if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
				{
                    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
					LOG_ERROR("ERROR::ASSIMP::{0}", importer.GetErrorString());
					return nullptr; // failure
				}
#else
				// separate model loading library
#pragma error "Define model loading library!"
#endif // AI_CONFIG_H_INC

				if (mesh)
				{
					Resources::AddMesh(GetFileNameNoExt(modelFilePath).c_str(), mesh);
					return Resources::GetMesh(GetFileNameNoExt(modelFilePath).c_str());
				}
				else
				{
					return Resources::GetMesh(null_mesh);
				}
			}
			else
			{
				return Resources::GetMesh(GetFileNameNoExt(modelFilePath).c_str());
			}
		}
	}

}
