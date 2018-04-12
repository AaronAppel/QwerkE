#ifndef _imgui_Editor_H_
#define _imgui_Editor_H_

#include "Editor.h"

class imgui_Editor : Editor
{
public:
	imgui_Editor();
	~imgui_Editor();

	void NewFrame();
	void Update();
	void Draw();

	void DrawSceneList();
	void DrawShaderEditor(ShaderProgram* shader);

private:

};

#endif // !_Editor_H_
