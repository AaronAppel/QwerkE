#pragma once

#include "QF_ComponentCamera.h"
#include "QF_ComponentInfo.h"
#include "QF_ComponentLight.h"
#include "QF_ComponentMesh.h"
#include "QF_ComponentScript.h"
#include "QF_ComponentTransform.h"

namespace QwerkE {

    using EntityComponentsList = TemplateArgumentList <
        ComponentCamera,
        ComponentInfo,
        ComponentMesh,
        ComponentTransform,
        ComponentScript
    >;

}
