#pragma once

#include "../../Entities/Components/Component.h"

class btRigidBody;

namespace QwerkE {

    class PhysicsComponent : public Component // TODO: Change/move to b3_PhysicsComponent
    {
    public:
        PhysicsComponent();
    };

}
