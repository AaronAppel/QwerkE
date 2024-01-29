#pragma once

#include <map>
#include <memory>

#include "QC_StringHelpers.h"

#include "QF_Enums.h"

namespace QwerkE {

	class Editor;

	namespace Engine
	{

		QwerkE::eEngineMessage Run(const std::map<const char*, const char*>& programArgPairs);
		void Stop();

		void NewFrame();
		void Update(float deltatime);
		void Draw();

	}

}
