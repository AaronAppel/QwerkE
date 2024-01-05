#include "Physics.h"
#include "../Resources/Resources.h"

#include "../DataManager/ConfigHelper.h"
#include "QF_Log.h"

namespace QwerkE {

    void Physics::Initialize()
    {
        const ConfigData config = ConfigHelper::GetConfigData();

        if (config.systems.PhysicsEnabled)
        {
            LibraryInitialize();

            // #TODO Review GetMesh() call
            // Request shapes from Resources
            // Resources::GetMesh("Cube_UVd.obj");
        }
        else
        {
            LOG_WARN("Physics is currently disabled.");
        }
    }
}
