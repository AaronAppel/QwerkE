#ifndef _ResourceViewer_H_
#define _ResourceViewer_H_

#include "../../QwerkE_Common/Libraries/imgui/imgui.h"
#include "../../QwerkE_Common/Libraries/glew/GL/glew.h"

#include <string>
#include <map>
#include <vector>

class ResourceManager;
class Mesh;
class ShaderProgram;
class Model;
class Scene;
class GameObject;
class FrameBufferObject;

class ResourceViewer
{
public:
	ResourceViewer();
	~ResourceViewer();

	void Draw();

private:
	ResourceManager * m_ResourceManager = nullptr;

	int m_CurrentResource = 1;

	const std::map<std::string, Mesh*>* m_Meshes = nullptr;
	const std::map<std::string, ShaderProgram*>* m_Shaders = nullptr;
	const std::map<std::string, GLuint>* m_Textures = nullptr;
	const std::map<std::string, Model*>* m_Models = nullptr;
	// const std::map<std::string, GLuint>* m_Fonts = nullptr;
	// const std::map<std::string, int>* m_Levels = nullptr;
	// const std::map<std::string, Sound*>* m_Sounds = nullptr;

	// Model/mesh viewing
	unsigned char m_ItemsPerRow = 4;
	ImVec2 m_ImageSize = ImVec2(64, 64);

	FrameBufferObject* m_FBO;
	std::vector<GLuint> m_ImageHandles;
	void DrawMeshThumbnails();
	std::vector<GLuint> m_ModelImageHandles;
	void DrawModelThumbnails();

	GameObject* m_Subject = nullptr;
	Scene* m_ViewerScene = nullptr;
};

#endif // !_ResourceViewer_H_
