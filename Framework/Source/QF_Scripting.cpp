#include "QF_Scripting.h"

#include "QF_ComponentScript.h"
#include "QF_EntityHandle.h"

#include "QF_Scriptable.h"
#include "QF_ScriptCamera.h"
#include "QF_ScriptPatrol.h"
#include "QF_ScriptTesting.h"

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

            default:
                ASSERT(false, "Unsupported script type!");
                break;
            }
            return nullptr;
        }

	}

}
