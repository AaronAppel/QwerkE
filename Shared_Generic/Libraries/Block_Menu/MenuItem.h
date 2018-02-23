#ifndef __MENUITEM_H_
#define __MENUITEM_H_

class Mesh;
class MenuWindow;
class ShaderProgram;

extern int g_WindowWidth, g_WindowHeight;

class MenuItem
{
public:
	MenuItem() {};
	MenuItem(Mesh* mesh);
	~MenuItem();

	virtual void Update(vec2 mousePos);

	virtual void Draw();

	/* Functionality */
	virtual void LeftClick();
	virtual void RightClick(vec2 mousePos) {};
	virtual void MiddleClick(vec2 mousePos) {};
	virtual void Hover() {};

	/* Getters + Setters */
	// Getters
	Mesh* GetMesh() { return m_Mesh; };
	ShaderProgram* GetShader() { return m_Shader; };
	vec2 GetSize() { return m_Size; };
	vec2 GetPosition() { return m_Position; };

	// Setters
	void SetMesh(Mesh* mesh) { m_Mesh = mesh; };
	void SetShader(ShaderProgram* shader) { m_Shader = shader; };
	void SetParentWindow(MenuWindow* window);
	void SetPosition(vec2 position);
	void SetRotation(float rotation);
	void SetSize(vec2 size);
	void SetColor(vec4 color) { m_OriginalColor = color; };
	void SetResolution(vec2 resolution) { m_Resolution = resolution; };

	void UpdateTransform(); // re-calculate tranform mat4
	// getScreenPos
	// getScreenSize

protected:
	// Dialog
	void ShowDialogWindow() {};
	bool m_ShowDialog = false;
	// Dialog* m_Dialog = nullptr;

	MenuWindow* m_ParentWindow = nullptr;

	// Graphics
	Mesh* m_Mesh = nullptr;
	ShaderProgram* m_Shader = nullptr;

	vec4 m_OriginalColor = vec4(0.2, 0.4,0.6f, 1.0f); // Used when no image
	vec4 m_Color = vec4(0.9f,0.9f,0.9f,1.0f); // Current colour

	vec2 m_Size = vec2(0.5f, 0.5f); // percentage of parent window size
	vec2 m_Position = vec2(0.5f, 0.5f); // percentage of parent window size for position
	vec2 m_Resolution = vec2(g_WindowWidth, g_WindowHeight); // TODO:: get resolution better
	float m_Rotation = 0.0f; // 2D rotation

	mat4 m_Transform;

	bool m_IsHighlighted = false;

	/* Overlay buttons */
	Linear2LinkedList<MenuItem*> m_OverLayButtons;
	void DrawOverlays();
};

#endif // __MENUITEM_H_