#ifndef __MYIMGUI_H__
#define __MYIMGUI_H__

class MyImGUI
{
public:
	MyImGUI();
	~MyImGUI();

	void Init(MyGLFW* window);

	void NewFrame();
	void Render();

private:
	bool show_test_window = true;
	bool show_another_window = false;
	bool show_window_3 = true;
	bool show_checkbox_1 = true;
	ImVec4 test_color = ImColor(114, 144, 154);
};

#endif //__MYIMGUI_H__
