#include "QF_Scripting.h"

#include "QF_ScriptHelpers.h"

// #TODO Fix test code
#include "../Game/QG_ScriptGameEntity.h"

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

            // #TODO Improve architecture for user generated types
            case eScriptTypes::UserScript1:
                return new ScriptGameEntity();

            // case eScriptTypes::UserScript1:
            //     return new ScriptablePathFinder();
            // case eScriptTypes::UserScript1:
            //     return new ScriptablePathFinder();

            default:
                ASSERT(false, "Unsupported script type!");
                break;
            }
            return nullptr;
        }

	}

}
