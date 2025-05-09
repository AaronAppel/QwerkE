
#include "Libraries/imgui/QC_imgui.h"

#include "Libraries/RCCppRuntimeObjectSystem/RuntimeObjectSystem/IObject.h"
#include "Libraries/RCCppRuntimeObjectSystem/RuntimeObjectSystem/IObjectFactorySystem.h"
#include "Libraries/RCCppRuntimeObjectSystem/RuntimeObjectSystem/IRuntimeObjectSystem.h"
#include "Libraries/RCCppRuntimeObjectSystem/RuntimeObjectSystem/ISimpleSerializer.h"
#include "Libraries/RCCppRuntimeObjectSystem/RuntimeObjectSystem/ObjectInterfacePerModule.h"
#include "Libraries/RCCppRuntimeObjectSystem/RuntimeObjectSystem/RuntimeSourceDependency.h"

#include "Libraries/RCCppQwerkE/StdioLogSystem.h"
#include "Libraries/RCCppQwerkE/SystemTable.h"

#include "RCCppMainLoop.h"

#include "Libraries/imgui/imgui.h"
RUNTIME_COMPILER_SOURCEDEPENDENCY_FILE("Libraries/imgui/imgui", ".cpp");
RUNTIME_COMPILER_SOURCEDEPENDENCY_FILE("Libraries/imgui/imgui_widgets", ".cpp");
RUNTIME_COMPILER_SOURCEDEPENDENCY_FILE("Libraries/imgui/imgui_tables", ".cpp");
RUNTIME_COMPILER_SOURCEDEPENDENCY_FILE("Libraries/imgui/imgui_draw", ".cpp");
RUNTIME_COMPILER_SOURCEDEPENDENCY_FILE("Libraries/imgui/imgui_demo", ".cpp");

// RCC++ uses interface Id's to distinguish between different classes
// here we have only one, so we don't need a header for this enum and put it in the same
// source code file as the rest of the code
enum InterfaceIDEnumConsoleExample
{
	IID_IRCCPP_MAIN_LOOP = IID_ENDInterfaceID, // IID_ENDInterfaceID from IObject.h InterfaceIDEnum

	IID_ENDInterfaceIDEnumConsoleExample
};

struct RCCppMainLoop : RCCppMainLoopI, TInterface<IID_IRCCPP_MAIN_LOOP, IObject>
{
	bool show_demo_window = false;

	// data for compiling window
	static constexpr double SHOW_AFTER_COMPILE_TIME = 3.0f;
	double compileStartTime = -SHOW_AFTER_COMPILE_TIME;
	double compileEndTime = -SHOW_AFTER_COMPILE_TIME;

	RCCppMainLoop()
	{
		PerModuleInterface::g_pSystemTable->pRCCppMainLoopI = this;
		g_pSys->pRuntimeObjectSystem->GetObjectFactorySystem()->SetObjectConstructorHistorySize(10);
	}

	void Init(bool isFirstInit) override
	{
		// If you want to do any initialization which is expensive and done after state
		// has been serialized you can do this here.

		if (isFirstInit)
		{
			// do any init needed to be done only once here, isFirstInit only set
			// when object is first constructed at program start.
		}
		// can do any initialization you might want to change here.
	}

	void Serialize(ISimpleSerializer* pSerializer) override
	{
		SERIALIZE(show_demo_window);
		SERIALIZE(compileStartTime);
		SERIALIZE(compileEndTime);
	}

	bool wasCompiling = false;
	void MainLoop() override
	{
		// ImGui::SetCurrentContext( PerModuleInterface::g_pSystemTable->pImContext );

		if (show_demo_window)
		{
			if (ImGui::GetCurrentContext())
			{
				ImGui::ShowDemoWindow(&show_demo_window);
			}
		}

		// Developer tools window
		bool doRCCppUndo = false;
		bool doRCCppRedo = false;

		bool bCompiling = g_pSys->pRuntimeObjectSystem->GetIsCompiling();
		if (bCompiling)
		{
			int bp = 01;
		}
		else
		{
			if (wasCompiling)
			{

			}

			if (ImGui::GetCurrentContext())
			{
				ImGui::DefaultDebugWindow([]() {
					ImGui::Text("Runtime Recompile aa");
					});
			}
		}

		// Do not add any code after this point as Undo/Redo will delete this
		if (doRCCppUndo)
		{
			g_pSys->pRuntimeObjectSystem->GetObjectFactorySystem()->UndoObjectConstructorChange();
		}
		if (doRCCppRedo)
		{
			g_pSys->pRuntimeObjectSystem->GetObjectFactorySystem()->RedoObjectConstructorChange();
		}

		wasCompiling = bCompiling;
	}
};

REGISTERSINGLETON(RCCppMainLoop, true);

