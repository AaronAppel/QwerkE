#pragma once

#include "QE_ComponentScript.h"

#include "QE_Scriptable.h"
#include "QE_Scripting.h"

#include "QE_ScriptCamera.h"
#include "QE_ScriptPathFinder.h"
#include "QE_ScriptPatrol.h"
#include "QE_ScriptTesting.h"
#include "QE_ScriptSceneTransition.h"

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
