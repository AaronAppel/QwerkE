#include "QF_Scripting.h"

#include "QF_ScriptHelpers.h"

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

            default:
                ASSERT(false, "Unsupported script type!");
                break;
            }
            return nullptr;
        }

	}

}
