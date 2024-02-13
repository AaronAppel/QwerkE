#pragma once

#include "QF_Enums.h"
#include "QF_GameObject.h"

namespace QwerkE {

    // #TODO Change the name to be more appropriate. Other factories exist such as MeshFactory and ShaderFactory.
    //  This class could be called EntityFactory and be moved to the System/Entities folder.

    class GameObject;
    class Scene;
    class RenderComponent;

    class Factory // #TODO Look to deprecate, or make editor only as it's really just for convenience
    {
    public:
        static GameObject* CreateSkyBox(Scene* scene, vec3 position);

        static GameObject* CreateNanosuit(Scene* scene, vec3 position);
        static GameObject* CreateTestCube(Scene* scene, vec3 position);
        static GameObject* CreateEmptyGameObject(Scene* scene, vec3 position);

    private:
        Factory() = default;

        static RenderComponent* AddModelComponentFromSchematic(GameObject* entity, const char* objectRecipeName);

    private:
        static int m_CreatedCount;
    };

}
