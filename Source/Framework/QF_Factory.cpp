#include "QF_Factory.h"

#include "QC_StringHelpers.h"

#include "QF_Defines.h"
#include "QF_FileUtilities.h"
#include "QF_GraphicsHelpers.h"
#include "QF_RenderComponent.h"
#include "QF_Resources.h"
#include "QF_Serialization_Schematics.h"

namespace QwerkE {

    int Factory::m_CreatedCount = 0;

    RenderComponent* Factory::AddModelComponentFromSchematic(GameObject* entity, const char* objectRecipeName)
    {
        RenderComponent* mComp = Serialization::LoadRenderComponentFromSchematic(ObjectSchematicsFolderPath(objectRecipeName));
        entity->AddComponent((Component*)mComp);
        return mComp;
    }

}
