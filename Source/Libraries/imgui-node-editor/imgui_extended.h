
// Copied from imgui.h/.cpp

// #define IMUI_EXTENDED_WIP

#ifdef IMUI_EXTENDED_WIP

#include "imgui.h"
#include "imgui_internal.h"

namespace ImGui
{
	IMGUI_API void BeginHorizontal(const char* str_id, const ImVec2& size = ImVec2(0, 0));
	IMGUI_API void BeginHorizontal(const void* ptr_id, const ImVec2& size = ImVec2(0, 0));
	IMGUI_API void EndHorizontal();
	IMGUI_API void BeginVertical(const char* str_id, const ImVec2& size = ImVec2(0, 0));
	IMGUI_API void BeginVertical(const void* ptr_id, const ImVec2& size = ImVec2(0, 0));
	IMGUI_API void EndVertical();
	IMGUI_API void Spring(float weight = 1.0f, float spacing = -1.0f);

	// Stack Layout
	static void             AddLayoutSpring(ImGuiLayout& layout, float weight, float spacing);
}

#endif
