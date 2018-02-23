#include "../main.h"

static void ShowExampleAppCustomNodeGraph(bool* nodeMenu);

MyImGUI::MyImGUI()
{
}

MyImGUI::~MyImGUI()
{
	ImGui_ImplGlfwGL3_Shutdown();
}

void MyImGUI::Init(MyGLFW* window)
{
	// Setup ImGui binding
#ifdef _glfw3_h_
	ImGui_ImplGlfwGL3_Init(window->GetWindow(), true);
#else
	OutputMessage("\nImGUI Uninitialized!");
	assert(0);
#endif

	// Load Fonts
	// (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
	//ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	int width, height;
	window->GetWindowSize(&width, &height);

	// Application init
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = (float)width;
	io.DisplaySize.y = (float)height;
	io.IniFilename = "imgui.ini";
	// io.RenderDrawListsFn = NULL;
	// io.RenderDrawListsFn = my_render_function;  // Setup a render function, or set to NULL and call GetDrawData() after Render() to access the render data.
	// TODO: Fill others settings of the io structure
}

void MyImGUI::NewFrame()
{
	ImGui_ImplGlfwGL3_NewFrame();
	// ImGui::NewFrame();
}

void MyImGUI::Render()
{

	int currentItem = 0;
	const char* items = { 0 };
	float inputFloat = 0.0f;

	// 0. Show a simple Menu Bar
	ImGui::BeginMainMenuBar();
	if (ImGui::Button("Menu")) {}
	ImGui::EndMainMenuBar();

	bool nodeMenu = true;
	// ShowExampleAppCustomNodeGraph(&nodeMenu);

	{
		bool IsTrue = true;
		ImGui::Begin("Image", &IsTrue, 0);
		//image1Size = ImVec2(ImGui::GetWindowSize().x - 30, ImGui::GetWindowSize().y - 30);
		//ImGui::Image(image1, image1Size);
		ImGui::End();
	}

	// 1. Show a simple window
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
	{
		static float f = 0.0f;
		ImGui::Text("Hello, world!");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::ColorEdit3("test color", (float*)&test_color);
		if (ImGui::Button("Test Window")) show_test_window ^= 1;
		if (ImGui::Button("Another Window")) show_another_window ^= 1;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	// 2. Show another simple window, this time using an explicit Begin/End pair
	if (show_another_window)
	{
		ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Another Window", &show_another_window);
		ImGui::Text("Hello");
		ImGui::End();
	}

	// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if (show_test_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}

	if (show_window_3)
	{
		if (720 - ImGui::GetMousePos().y > 680)
		{
			ImVec2 temp = ImGui::GetMouseDragDelta();
			//windowPos.x = temp.x + windowPos.x;
			//windowPos.y = temp.y + windowPos.y;
			//glfwSetWindowPos(window, windowPos.x, windowPos.y);
		}


		ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Window 3", &show_window_3);

		// if (ImGui::Button("Shutdown")) g_MainWindow->RequestClose();

		if (ImGui::TreeNode("Bullets"))
		{
			ImGui::BulletText("Bullet point 1");
			ImGui::BulletText("Bullet point 2\nOn multiple lines");
			ImGui::Bullet(); ImGui::Text("Bullet point 3 (two calls)");
			ImGui::Bullet(); ImGui::SmallButton("Button");
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Word Wrapping"))
		{
			// Using shortcut. You can use PushTextWrapPos()/PopTextWrapPos() for more flexibility.
			ImGui::TextWrapped("This text should automatically wrap on the edge of the window. The current implementation for text wrapping follows simple rules suitable for English and possibly other languages.");
			ImGui::Spacing();

			static float wrap_width = 200.0f;
			ImGui::SliderFloat("Wrap width", &wrap_width, -20, 600, "%.0f");

			ImGui::Text("Test paragraph 1:");
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x + wrap_width, pos.y), ImVec2(pos.x + wrap_width + 10, pos.y + ImGui::GetTextLineHeight()), IM_COL32(255, 0, 255, 255));
			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
			ImGui::Text("The lazy dog is a good dog. This paragraph is made to fit within %.0f pixels. Testing a 1 character word. The quick brown fox jumps over the lazy dog.", wrap_width);
			ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 0, 255));
			ImGui::PopTextWrapPos();

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Grid"))
		{
			static bool selected[16] = { true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true };
			for (int i = 0; i < 16; i++)
			{
				ImGui::PushID(i);
				if (ImGui::Selectable("Sailor", &selected[i], 0, ImVec2(50, 50)))
				{
					int x = i % 4, y = i / 4;
					if (x > 0) selected[i - 1] ^= 1;
					if (x < 3) selected[i + 1] ^= 1;
					if (y > 0) selected[i - 4] ^= 1;
					if (y < 3) selected[i + 4] ^= 1;
				}
				if ((i % 4) < 3) ImGui::SameLine();
				ImGui::PopID();
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Vertical Sliders"))
		{
			ImGui::Unindent();
			const float spacing = 4;
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));

			static int int_value = 0;
			ImGui::VSliderInt("##int", ImVec2(18, 160), &int_value, 0, 5);
			ImGui::SameLine();

			static float values[7] = { 0.0f, 0.60f, 0.35f, 0.9f, 0.70f, 0.20f, 0.0f };
			ImGui::PushID("set1");
			for (int i = 0; i < 7; i++)
			{
				if (i > 0) ImGui::SameLine();
				ImGui::PushID(i);
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor::HSV(i / 7.0f, 0.5f, 0.5f));
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImColor::HSV(i / 7.0f, 0.6f, 0.5f));
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImColor::HSV(i / 7.0f, 0.7f, 0.5f));
				ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImColor::HSV(i / 7.0f, 0.9f, 0.9f));
				ImGui::VSliderFloat("##v", ImVec2(18, 160), &values[i], 0.0f, 1.0f, "");
				if (ImGui::IsItemActive() || ImGui::IsItemHovered())
					ImGui::SetTooltip("%.3f", values[i]);
				ImGui::PopStyleColor(4);
				ImGui::PopID();
			}
			ImGui::PopID();

			ImGui::SameLine();
			ImGui::PushID("set2");
			static float values2[4] = { 0.20f, 0.80f, 0.40f, 0.25f };
			const int rows = 3;
			const ImVec2 small_slider_size(18, (160.0f - (rows - 1)*spacing) / rows);
			for (int nx = 0; nx < 4; nx++)
			{
				if (nx > 0) ImGui::SameLine();
				ImGui::BeginGroup();
				for (int ny = 0; ny < rows; ny++)
				{
					ImGui::PushID(nx*rows + ny);
					ImGui::VSliderFloat("##v", small_slider_size, &values2[nx], 0.0f, 1.0f, "");
					if (ImGui::IsItemActive() || ImGui::IsItemHovered())
						ImGui::SetTooltip("%.3f", values2[nx]);
					ImGui::PopID();
				}
				ImGui::EndGroup();
			}
			ImGui::PopID();

			ImGui::SameLine();
			ImGui::PushID("set3");
			for (int i = 0; i < 4; i++)
			{
				if (i > 0) ImGui::SameLine();
				ImGui::PushID(i);
				ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 40);
				ImGui::VSliderFloat("##v", ImVec2(40, 160), &values[i], 0.0f, 1.0f, "%.2f\nsec");
				ImGui::PopStyleVar();
				ImGui::PopID();
			}
			ImGui::PopID();
			ImGui::PopStyleVar();

			ImGui::Indent();
			ImGui::TreePop();
		}

		ImGui::Button("Label");
		ImGui::Text("Hello");

		ImGui::BulletText("Text");
		static float flt = 0;
		ImGui::Value("Value:", flt);
		ImGui::SliderAngle("Angle", &flt);

		ImGui::Separator();

		static unsigned int flags = 0;
		ImGui::Checkbox("Label", &show_checkbox_1);
		ImGui::CheckboxFlags("Label", &flags, 1);

		//ImGui::Combo("Label", &currentItem, items);

		static float drag = 0;
		ImGui::DragFloat("Label", &drag);

		ImGui::DragFloat2("Drag Float:", &drag);

		ImGui::InputFloat("InputFloat", &inputFloat);
		//ImGui::InputText("InputText", buffer, size);
		//ImGui::InputTextMultiline("Text", buffer, size);

		static float color;
		ImGui::ColorEdit3("ColorEdit:", &color);

		ImGui::ColorButton(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
		ImGui::End();
	}
		// RENDER
		ImGui::Render();
}

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

void ShowExampleAppCustomNodeGraph(bool* nodeMenu)
{
	ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiSetCond_FirstUseEver);
	if (!ImGui::Begin("Example: Custom Node Graph", nodeMenu))
	{
		ImGui::End();
		return;
	}

	// Dummy
	struct Node
	{
		int     ID;
		char    Name[32];
		ImVec2  Pos, Size;
		float   Value;
		ImVec4  Color;
		int     InputsCount, OutputsCount;

		Node(int id, const char* name, const ImVec2& pos, float value, const ImVec4& color, int inputs_count, int outputs_count) { ID = id; strncpy_s(Name, name, 31); Name[31] = 0; Pos = pos; Value = value; Color = color; InputsCount = inputs_count; OutputsCount = outputs_count; }

		ImVec2 GetInputSlotPos(int slot_no) const { return ImVec2(Pos.x, Pos.y + Size.y * ((float)slot_no + 1) / ((float)InputsCount + 1)); }
		ImVec2 GetOutputSlotPos(int slot_no) const { return ImVec2(Pos.x + Size.x, Pos.y + Size.y * ((float)slot_no + 1) / ((float)OutputsCount + 1)); }
	};
	struct NodeLink
	{
		int     InputIdx, InputSlot, OutputIdx, OutputSlot;

		NodeLink(int input_idx, int input_slot, int output_idx, int output_slot) { InputIdx = input_idx; InputSlot = input_slot; OutputIdx = output_idx; OutputSlot = output_slot; }
	};

	static ImVector<Node> nodes;
	static ImVector<NodeLink> links;
	static bool inited = false;
	static ImVec2 scrolling = ImVec2(0.0f, 0.0f);
	static bool show_grid = true;
	static int node_selected = -1;
	if (!inited)
	{
		nodes.push_back(Node(0, "MainTex", ImVec2(40, 50), 0.5f, ImColor(255, 100, 100), 1, 1));
		nodes.push_back(Node(1, "BumpMap", ImVec2(40, 150), 0.42f, ImColor(200, 100, 200), 1, 1));
		nodes.push_back(Node(2, "Combine", ImVec2(270, 80), 1.0f, ImColor(0, 200, 100), 2, 2));
		links.push_back(NodeLink(0, 0, 2, 0));
		links.push_back(NodeLink(1, 0, 2, 1));
		inited = true;
	}

	// Draw a list of nodes on the left side
	bool open_context_menu = false;
	int node_hovered_in_list = -1;
	int node_hovered_in_scene = -1;
	ImGui::BeginChild("node_list", ImVec2(100, 0));
	ImGui::Text("Nodes");
	ImGui::Separator();
	for (int node_idx = 0; node_idx < nodes.Size; node_idx++)
	{
		Node* node = &nodes[node_idx];
		ImGui::PushID(node->ID);
		if (ImGui::Selectable(node->Name, node->ID == node_selected))
			node_selected = node->ID;
		if (ImGui::IsItemHovered())
		{
			node_hovered_in_list = node->ID;
			open_context_menu |= ImGui::IsMouseClicked(1);
		}
		ImGui::PopID();
	}
	ImGui::EndChild();

	ImGui::SameLine();
	ImGui::BeginGroup();

	const float NODE_SLOT_RADIUS = 4.0f;
	const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);

	// Create our child canvas
	ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", scrolling.x, scrolling.y);
	ImGui::SameLine(ImGui::GetWindowWidth() - 100);
	ImGui::Checkbox("Show grid", &show_grid);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor(60, 60, 70, 200));
	ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
	ImGui::PushItemWidth(120.0f);

	ImVec2 offset = ImGui::GetCursorScreenPos() - scrolling;
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->ChannelsSplit(2);

	// Display grid
	if (show_grid)
	{
		ImU32 GRID_COLOR = ImColor(200, 200, 200, 40);
		float GRID_SZ = 64.0f;
		ImVec2 win_pos = ImGui::GetCursorScreenPos();
		ImVec2 canvas_sz = ImGui::GetWindowSize();
		for (float x = fmodf(offset.x, GRID_SZ); x < canvas_sz.x; x += GRID_SZ)
			draw_list->AddLine(ImVec2(x, 0.0f) + win_pos, ImVec2(x, canvas_sz.y) + win_pos, GRID_COLOR);
		for (float y = fmodf(offset.y, GRID_SZ); y < canvas_sz.y; y += GRID_SZ)
			draw_list->AddLine(ImVec2(0.0f, y) + win_pos, ImVec2(canvas_sz.x, y) + win_pos, GRID_COLOR);
	}

	// Display links
	draw_list->ChannelsSetCurrent(0); // Background
	for (int link_idx = 0; link_idx < links.Size; link_idx++)
	{
		NodeLink* link = &links[link_idx];
		Node* node_inp = &nodes[link->InputIdx];
		Node* node_out = &nodes[link->OutputIdx];
		ImVec2 p1 = offset + node_inp->GetOutputSlotPos(link->InputSlot);
		ImVec2 p2 = offset + node_out->GetInputSlotPos(link->OutputSlot);
		draw_list->AddBezierCurve(p1, p1 + ImVec2(+50, 0), p2 + ImVec2(-50, 0), p2, ImColor(200, 200, 100), 3.0f);
	}

	// Display nodes
	for (int node_idx = 0; node_idx < nodes.Size; node_idx++)
	{
		Node* node = &nodes[node_idx];
		ImGui::PushID(node->ID);
		ImVec2 node_rect_min = offset + node->Pos;

		// Display node contents first
		draw_list->ChannelsSetCurrent(1); // Foreground
		bool old_any_active = ImGui::IsAnyItemActive();
		ImGui::SetCursorScreenPos(node_rect_min + NODE_WINDOW_PADDING);
		ImGui::BeginGroup(); // Lock horizontal position
		ImGui::Text("%s", node->Name);
		ImGui::SliderFloat("##value", &node->Value, 0.0f, 1.0f, "Alpha %.2f");
		ImGui::ColorEdit3("##color", &node->Color.x);
		ImGui::EndGroup();

		// Save the size of what we have emitted and whether any of the widgets are being used
		bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
		node->Size = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
		ImVec2 node_rect_max = node_rect_min + node->Size;

		// Display node box
		draw_list->ChannelsSetCurrent(0); // Background
		ImGui::SetCursorScreenPos(node_rect_min);
		ImGui::InvisibleButton("node", node->Size);
		if (ImGui::IsItemHovered())
		{
			node_hovered_in_scene = node->ID;
			open_context_menu |= ImGui::IsMouseClicked(1);
		}
		bool node_moving_active = ImGui::IsItemActive();
		if (node_widgets_active || node_moving_active)
			node_selected = node->ID;
		if (node_moving_active && ImGui::IsMouseDragging(0))
			node->Pos = node->Pos + ImGui::GetIO().MouseDelta;

		ImU32 node_bg_color = (node_hovered_in_list == node->ID || node_hovered_in_scene == node->ID || (node_hovered_in_list == -1 && node_selected == node->ID)) ? ImColor(75, 75, 75) : ImColor(60, 60, 60);
		draw_list->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
		draw_list->AddRect(node_rect_min, node_rect_max, ImColor(100, 100, 100), 4.0f);
		for (int slot_idx = 0; slot_idx < node->InputsCount; slot_idx++)
			draw_list->AddCircleFilled(offset + node->GetInputSlotPos(slot_idx), NODE_SLOT_RADIUS, ImColor(150, 150, 150, 150));
		for (int slot_idx = 0; slot_idx < node->OutputsCount; slot_idx++)
			draw_list->AddCircleFilled(offset + node->GetOutputSlotPos(slot_idx), NODE_SLOT_RADIUS, ImColor(150, 150, 150, 150));

		ImGui::PopID();
	}
	draw_list->ChannelsMerge();

	// Open context menu
	if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(1))
	{
		node_selected = node_hovered_in_list = node_hovered_in_scene = -1;
		open_context_menu = true;
	}
	if (open_context_menu)
	{
		ImGui::OpenPopup("context_menu");
		if (node_hovered_in_list != -1)
			node_selected = node_hovered_in_list;
		if (node_hovered_in_scene != -1)
			node_selected = node_hovered_in_scene;
	}

	// Draw context menu
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
	if (ImGui::BeginPopup("context_menu"))
	{
		Node* node = node_selected != -1 ? &nodes[node_selected] : NULL;
		ImVec2 scene_pos = ImGui::GetMousePosOnOpeningCurrentPopup() - offset;
		if (node)
		{
			ImGui::Text("Node '%s'", node->Name);
			ImGui::Separator();
			if (ImGui::MenuItem("Rename..", NULL, false, false)) {}
			if (ImGui::MenuItem("Delete", NULL, false, false)) {}
			if (ImGui::MenuItem("Copy", NULL, false, false)) {}
		}
		else
		{
			if (ImGui::MenuItem("Add")) { nodes.push_back(Node(nodes.Size, "New node", scene_pos, 0.5f, ImColor(100, 100, 200), 2, 2)); }
			if (ImGui::MenuItem("Paste", NULL, false, false)) {}
		}
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();

	// Scrolling
	if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
		scrolling = scrolling - ImGui::GetIO().MouseDelta;

	ImGui::PopItemWidth();
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
	ImGui::EndGroup();

	ImGui::End();
}