#include "stdafx.h"

// TODO: Load all files in folder. This avoids hard coded assets names and allows easy adding/removal of assets.
// Objects may need to switch to assets ids. ids would act as indices in the asset list and would prevent crashing if assets are removed.

void ResourceManager::Init()
{
}

// Instantiation Functions
// TODO: Handle errors and deleting assets before returning nullptr
Mesh* ResourceManager::InstantiateMesh(const char* meshName)
{
	MeshFactory t_MeshFactory;
	Mesh* mesh = nullptr;
    // TODO: Dereference *s
	if (meshName == "Box") // Asset name
	{
		mesh = new Mesh();
		t_MeshFactory.GenerateBox(mesh, vec2(1, 1));
	}
	else if (meshName == "Cube")
	{
		mesh = new Mesh();
		t_MeshFactory.GenerateCube(mesh, vec3(1, 1, 1));
	}
	else if (meshName == "CreBox")
	{
		mesh = t_MeshFactory.CreateBox(vec2(1, 1));
	}
	else if (meshName == "CreCube")
	{
		mesh = t_MeshFactory.CreateCube(vec3(1, 1, 1), vec2(1, 1), true);
	}
	else if (meshName == "Plane")
	{
		mesh = t_MeshFactory.CreatePlane(vec2(10, 10), vec2(2, 2), vec2(1, 1));
	}
	else if (meshName == "TutorialCube")
	{
		mesh = t_MeshFactory.TutorialCube(vec3(1,1,1));
	}
	else
	{
		return nullptr;
	}
	m_HotMeshes[meshName] = mesh; // Add to active list
	mesh->SetName(meshName);
	return mesh;
}

ShaderProgram* ResourceManager::InstantiateShader(const char* shaderName)
{
	ShaderProgram* shader = new ShaderProgram();
	// 2D
	if (shaderName == "Basic2DTex") // Asset name
	{
		shader->Init("Resources/Shaders/Basic2DTex.vert", "Resources/Shaders/Basic2DTex.frag", NULL); // Asset directories
	}
	else if (shaderName == "2DMenu")
	{
		shader->Init("Resources/Shaders/2DMenu.vert", "Resources/Shaders/2DMenu.frag", NULL);
	}
	else if (shaderName == "Basic2D")
	{
		// shader->Init(eShader_Basic2D);
	}
	else if (shaderName == "2DMenuText")
	{
		shader->Init("Resources/Shaders/2DMenuText.vert", "Resources/Shaders/2DMenuText.frag", NULL);
	}
	else if (shaderName == "Basic2DTransform")
	{
		shader->Init("Resources/Shaders/Basic2DTransform.vert", "Resources/Shaders/Basic2DTransform.frag", NULL);
	}
	else if (shaderName == "Sprite2D")
	{
		shader->Init("Resources/Shaders/Sprite2D.vert", "Resources/Shaders/Sprite2D.frag", NULL);
	}
	// 3D
	else if (shaderName == "Basic3D")
	{
		shader->Init(eShader_Basic3D);
	}
	else if (shaderName == "LitMaterial")
	{
		shader->Init("Resources/Shaders/LitMaterial.vert", "Resources/Shaders/LitMaterial.frag", NULL);
	}
	else if (shaderName == "vec3Material")
	{
		shader->Init("Resources/Shaders/vec3Material.vert", "Resources/Shaders/vec3Material.frag", NULL);
	}
	else if (shaderName == "BasicLighting")
	{
		shader->Init("Resources/Shaders/BasicLighting.vert", "Resources/Shaders/BasicLighting.frag", NULL);
	}
	else if (shaderName == "Box2D_Debug")
	{
		// InstantiateShader("Data/Shaders/Box2DDebug.vert", "Data/Shaders/Box2DDebug.frag", NULL, "Box2D_Debug");
	}
	else
	{
		return nullptr;
	}

	m_HotShaders[shaderName] = shader;
	return shader;
}

GLuint ResourceManager::InstantiateTexture(const char* textureName)
{
	GLuint textureHandle = -1;
	if (textureName == "PeriodicHeal") // Asset name
	{
		textureHandle = Load2DTexture("Resources/Textures/PeriodicHeal.png"); // Asset directory
	}
	else if (textureName == "FlashHeal")
	{
		textureHandle = Load2DTexture("Resources/Textures/FlashHeal.png");
	}
	else if (textureName == "ExampleBackground")
	{
		textureHandle = Load2DTexture("Resources/Textures/ExampleBackground.png");
	}
	else if (textureName == "container")
	{
		textureHandle = Load2DTexture("Resources/Textures/container.png");
	}
	else if (textureName == "container_specular")
	{
		textureHandle = Load2DTexture("Resources/Textures/container_specular.png");
	}
	else if (textureName == "Blue_Engine_UI1")
	{
		textureHandle = Load2DTexture("Resources/Textures/Blue_Engine_UI1.png");
	}
	else if (textureName == "Blue_Engine_UI2")
	{
		textureHandle = Load2DTexture("Resources/Textures/Blue_Engine_UI2.png");
	}
	else if (textureName == "UV_Map")
	{
		textureHandle = Load2DTexture("Resources/Textures/UVMap.png");
	}
	else if (textureName == "Menu_Border1")
	{
		textureHandle = Load2DTexture("Resources/Textures/Menu_Border1.png");
	}
	else if (textureName == "arm_dif")
	{
		textureHandle = Load2DTexture("Resources/Models/Crysis_Nanosuit/tex/arm_dif.png");
	}
	else if (textureName == "arm_spec")
	{
		textureHandle = Load2DTexture("Resources/Models/Crysis_Nanosuit/tex/arm_spec.png");
	}
	else if (textureName == "white_canvas")
	{
		textureHandle = Load2DTexture("Resources/Textures/white_canvas.png");
	}
	else if (textureName == "null")
	{
		textureHandle = Load2DTexture("Resources/Textures/null_texture.png");
	}
	// Cube map
	else if (false)
	{

	}
	else
	{
		textureHandle = -1; // failure
	}

	m_HotTextures[textureName] = textureHandle;
	return textureHandle;
}

MaterialData* ResourceManager::InstantiateMaterial(const char* matName)
{
	MaterialData* material = nullptr;
	if (matName == "container") // Asset name
	{
		material = new MaterialData(GetTexture("container"), GetTexture("container"), GetTexture("container_specular")); // Asset names
	}
	else if (matName == "nanosuit_visor")
	{
		// material = new MaterialData(GetTexture("container"), GetTexture("container"), GetTexture("container_specular"));
	}
	else if (matName == "nanosuit_legs")
	{
		// material = new MaterialData(GetTexture("container"), GetTexture("container"), GetTexture("container_specular"));
	}
	else if (matName == "UV_Map")
	{
		material = new MaterialData(GetTexture("UV_Map"), GetTexture("UV_Map"), GetTexture("white_canvas"));
	}
	else
	{
		return nullptr;
	}

	m_HotMaterials[matName] = material;
	material->name = matName;
	return material;
}

Model* ResourceManager::InstantiateModel(const char* modelName)
{
	Model* model;
	if (modelName == "LightBulb") // Asset name
	{
		model = ImportOBJModel("Resources/Models/Light_Bulb.obj"); // Asset directory
	}
	else if (modelName == "CubeModel")
	{
		model = ImportOBJModel("Resources/Models/1M_CubeModel.obj", vec3(0.05f, 0.05f, 0.05f));
	}
	else if (modelName == "Pyramid")
	{
		model = ImportOBJModel("Resources/Models/Pyramid.obj");
	}
	else if (modelName == "Nano_Suit")
	{
		model = ImportOBJModel("Resources/Models/Crysis_Nanosuit/nanosuit.obj");
	}
	else if (modelName == "NullMesh")
	{
		model = ImportOBJModel("Resources/Models/Teapot.obj");
	}
	else
	{
		return nullptr;
	}
	// TODO: nullptr checks
	m_HotModels[modelName] = model;
	model->SetName(modelName);
	return model;
}
