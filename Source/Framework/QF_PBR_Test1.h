#pragma once

#include <vector>

#include "QF_Scene.h"

namespace QwerkE {

    // #TODO Deprecate PBR test scene
    // #TODO Maybe a code file would be useful for generating a scene, or improve editor, but this class shouldn't exist
    class PBR_Test1 : public Scene
    {
    public:
        PBR_Test1();

        void Update(float deltatime) override;

        void Initialize() override;

    private:
        std::vector<GameObject*> m_Subjects;
    };

}
