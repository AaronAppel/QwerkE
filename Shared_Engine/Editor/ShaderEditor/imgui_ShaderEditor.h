#ifndef _imgui_ShaderEditor_H_
#define _imgui_ShaderEditor_H_

class ShaderProgram;

class imgui_ShaderEditor
{
public:
	imgui_ShaderEditor();
	~imgui_ShaderEditor();

	void NewFrame();
	void Update();
	void Draw();

	void SetShader(ShaderProgram* shader) { m_Shader = shader; };
	const ShaderProgram* GetShader() { return m_Shader; };

private:
	ShaderProgram* m_Shader = nullptr;
};

#endif // !_imgui_ShaderEditor_H_
