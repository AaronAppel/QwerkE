#pragma once

#include "QF_ComponentAudio.h"
#include "QF_ComponentCamera.h"
#include "QF_ComponentInfo.h"
#include "QF_ComponentLight.h"
#include "QF_ComponentMesh.h"
#include "QF_ComponentPhysics.h"
#include "QF_ComponentScript.h"
#include "QF_ComponentTransform.h"

namespace QwerkE {

    using EntityComponentsList = TemplateArgumentList <

        // In order as they appear in Editor entity inspector window
        ComponentInfo,
        ComponentTransform,

        ComponentAudio,
        ComponentCamera,
        ComponentLight,
        ComponentMesh,
        ComponentScript,
        ComponentPhysics
        // #NOTE Add new Component/Components here
    >;

}
