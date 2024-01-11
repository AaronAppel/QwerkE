#include "QF_Factory.h"

#include "QC_StringHelpers.h"

#include "QF_Defines.h"
#include "QF_FileUtilities.h"
#include "QF_Resources.h"
#include "QF_RenderComponent.h"
#include "QF_GraphicsHelpers.h"

namespace QwerkE {

    int Factory::m_CreatedCount = 0;

    GameObject* Factory::CreateGameObject(Scene* scene)
    {
        return new GameObject(scene);
    }

    GameObject* Factory::CreateGameObject(Scene* scene, vec3 position)
    {
        return new GameObject(scene, position);
    }

    GameObject* Factory::CreateObjectFromSchematic(const char* schematicName, Scene* scene, vec3 position)
    {
        GameObject* object = CreateGameObject(scene, position);
        object->SetName(GetFileNameWithExt(schematicName));
        object->AddComponent(LoadRenderComponentFromSchematic(ObjectSchematicsFolderPath(schematicName)));
        return object;
    }

    RenderComponent* Factory::AddModelComponentFromSchematic(GameObject* entity, const char* objectRecipeName)
    {
        RenderComponent* mComp = LoadRenderComponentFromSchematic(ObjectSchematicsFolderPath(objectRecipeName));
        entity->AddComponent((Component*)mComp);
        return mComp;
    }

}
