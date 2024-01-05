#pragma once

#include "QF_Scene.h"

namespace QwerkE {

    class TestScene : public Scene
    {
    public:
        TestScene();

        void Update(float deltaTime);

        void Initialize();
    };

}
