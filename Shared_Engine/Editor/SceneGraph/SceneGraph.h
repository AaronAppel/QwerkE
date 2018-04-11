#ifndef _SceneGraph_H_
#define _SceneGraph_H_

// A window that shows entities and their hierarchal relationship

class SceneManager;
class ResourceManager;

class SceneGraph
{
public:
	SceneGraph();
    ~SceneGraph();

	void Draw();

private:
	SceneManager * m_SceneManager = nullptr;
	ResourceManager * m_ResourceManager = nullptr;
};

#endif // !_Editor_H_
