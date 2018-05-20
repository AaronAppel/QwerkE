#ifndef _imgui_EditComponent_H_
#define _imgui_EditComponent_H_

class GameObject;
class ResourceManager;
class Model;
class ShaderProgram;
struct MaterialData;

class imgui_EditComponent
{
public:
	imgui_EditComponent();
	~imgui_EditComponent();

	void Draw(GameObject* entity);

private:
	ResourceManager* m_ResourceManager = nullptr;

	bool m_RefreshFlag = 1;
	int m_MatIndex = 0;
	int m_ShaderIndex = 0;
	int m_TextureIndex = 0;
	int m_ModelIndex = 0;

	const std::map<std::string, MaterialData*>* m_Materials = nullptr;
	const std::map<std::string, ShaderProgram*>* m_Shaders = nullptr;
	const std::map<std::string, GLuint>* m_Textures = nullptr;
	const std::map<std::string, Model*>* m_Models = nullptr;

	std::vector<const char*> m_MatStrings;
	std::vector<const char*> m_ShaderStrings;
	std::vector<const char*> m_TextureStrings;
	std::vector<const char*> m_ModelStrings;
};

#endif // !_imgui_EditComponent_H_
