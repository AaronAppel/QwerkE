#pragma once

#include "QF_ComponentScript.h"

#include "QF_Scriptable.h"
#include "QF_Scripting.h"

#include "QF_ScriptCamera.h"
#include "QF_ScriptPathFinder.h"
#include "QF_ScriptPatrol.h"
#include "QF_ScriptTesting.h"
#include "QF_ScriptSceneTransition.h"

// #include "../Game/QG_ScriptGameEntity.h" // #TODO Remove Game code from Framework domain

namespace QwerkE {

    using ComponentScriptsList = TemplateArgumentList <
        // #NOTE Order matters! Match enum eScriptTypes order
        ScriptableCamera,
        ScriptablePathFinder,
        ScriptablePatrol,
        ScriptableTesting,
        ScriptableSceneTransition
        // ScriptGameEntity
        // #NOTE Add new Scriptable classes here
    >;


}
