#include "QC_imgui.h"

namespace ImGui
{

	ImGuiKey KeyCodeToImGuiKey(int keyCode)
	{
		if (keyCode == 65)
		{
			return ImGuiKey_A;
		}
		return ImGuiKey_Z;
	}

}