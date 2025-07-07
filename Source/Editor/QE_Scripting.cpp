#include "QE_Scripting.h"

#include "QE_ScriptHelpers.h"

// #TODO Fix test code
// #include "../Game/QG_ScriptGameEntity.h"

namespace QwerkE {

	namespace Scripting {

        Scriptable* InstantiateScript(eScriptTypes scriptType)
        {
            switch (scriptType)
            {
            case eScriptTypes::Camera:
                return new ScriptableCamera();

            case eScriptTypes::Testing:
                return new ScriptableTesting();

            case eScriptTypes::Patrol:
                return new ScriptablePatrol();

            case eScriptTypes::PathFinder:
                return new ScriptablePathFinder();

            case eScriptTypes::SceneTransition:
                return new ScriptableSceneTransition();

            // #TODO Improve architecture for user generated types. Ideally don't modify framework files
            case eScriptTypes::UserScript1:
                // return new ScriptGameEntity();
                // #TODO Review ScriptGameEntity();

            default:
                ASSERT(false, "Unsupported script type!");
                break;
            }
            return nullptr;
        }

	}

}
