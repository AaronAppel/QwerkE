#ifndef _imgui_EditComponent_H_
#define _imgui_EditComponent_H_

class GameObject;
class ResourceManager;
class ShaderProgram;
struct MaterialData;
class RenderComponent;
class Mesh;

class imgui_EditComponent
{
public:
	imgui_EditComponent();
	~imgui_EditComponent();

	void Draw(GameObject* entity);

private:
	ResourceManager* m_ResourceManager = nullptr;

	bool m_RefreshFlag = 1;

	const std::map<std::string, MaterialData*>* m_Materials = nullptr;
	const std::map<std::string, ShaderProgram*>* m_Shaders = nullptr;
	const std::map<std::string, GLuint>* m_Textures = nullptr;
	const std::map<std::string, Mesh*>* m_Meshes = nullptr;

	std::vector<const char*> m_ShaderStrings;
	std::vector<const char*> m_MatStrings;
	std::vector<const char*> m_TextureStrings;
	std::vector<const char*> m_MeshStrings;

	int m_RenderableIndex = 0; // current renderable selected

	bool m_ShowShaderList = true;
	int m_Shaderindex = 0; // deprecated
	bool m_ShowMaterialList = true;
	int m_Materialindex = 0; // deprecated
	bool m_ShowMeshList = true;
	int m_Meshindex = 0; // deprecated

	void ShowShaderMenu(RenderComponent* rComp);
	void ShowMaterialMenu(RenderComponent* rComp);
	void ShowMeshMenu(RenderComponent* rComp);
};

#endif // !_imgui_EditComponent_H_
