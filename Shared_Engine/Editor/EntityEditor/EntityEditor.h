#ifndef _EntityEditor
#define _EntityEditor

// A window that shows data for an entity and gives a GUI
// to allow for data to easily be modified

// TODO: Create an imgui_EntityEditor class
class SceneManager;
class ResourceManager;
class Editor;
class imgui_EditComponent;

#include <string>

// TODO: Change this to be an abstract base class that gets inherited by library version like imgui or BlockMenu
class EntityEditor // abstract
{
public:
    EntityEditor(Editor* editor);
    ~EntityEditor();

    virtual void Draw();

    void SetCurrentEntity(std::string entityName) { m_CurrentEntity = entityName; };

private:
    Editor* m_Editor = nullptr;
    SceneManager * m_SceneManager = nullptr;
    ResourceManager * m_ResourceManager = nullptr;

    std::string m_CurrentEntity = "Null";
	imgui_EditComponent* m_EditComponent = nullptr;
};

#endif // !_EntityEditor
