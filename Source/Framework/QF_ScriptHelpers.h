#pragma once

#include "QF_ComponentScript.h"

#include "QF_Scriptable.h"
#include "QF_Scripting.h"

#include "QF_ScriptCamera.h"
#include "QF_ScriptPathFinder.h"
#include "QF_ScriptPatrol.h"
#include "QF_ScriptTesting.h"
#include "QF_ScriptSceneTransition.h"

namespace QwerkE {

    using ComponentScriptsList = TemplateArgumentList <
        // #NOTE Order matters! Match enum eScriptTypes order
        ScriptableCamera,
        ScriptablePathFinder,
        ScriptablePatrol,
        ScriptableTesting,
        ScriptableSceneTransition
    >;


}
