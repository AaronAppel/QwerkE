#pragma once

#include <unordered_map>
#include <vector>

#include "QF_Input.h"
#include "QF_InputMapping.h"

namespace QwerkE {

	namespace Editor {

        enum EditorHotKeys : uint16_t
        {
            None = 0,

            // Scenes
            e_Scene_Select1 = 1000,
            e_Scene_Select2,
            e_Scene_Select3,
            e_Scene_Select4,
            e_Scene_Select5,
            e_Scene_Select6,
            e_Scene_Select7,
            e_Scene_Select8,
            e_Scene_Select9,
            e_Scene_Select10,
            e_Scene_Select11,
            e_Scene_Select12,
            e_Scene_SelectMax,
        };

	}

}