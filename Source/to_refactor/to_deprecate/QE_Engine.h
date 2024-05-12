#pragma once

#include <map>

#include "QF_Enums.h"

namespace QwerkE {

	namespace Engine { // #TODO Deprecate engine for editor

		QwerkE::eOperationResult Run(const std::map<const char*, const char*>& programArgPairs);
		void Stop();

		void NewFrame();
		void Update(float deltatime);
		void Draw();

	}

}
