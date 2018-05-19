#ifndef _imgui_EditComponent_H_
#define _imgui_EditComponent_H_

class GameObject;
class ResourceManager;

class imgui_EditComponent
{
public:
	imgui_EditComponent();
	~imgui_EditComponent();

	void Draw(GameObject* entity);

private:
	ResourceManager* m_ResourceManager = nullptr;
};

#endif // !_imgui_EditComponent_H_
