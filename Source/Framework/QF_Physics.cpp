#include "QF_Physics.h"

#include "QF_Resources.h"

#include "QF_Log.h"
#include "QF_Settings.h"

namespace QwerkE {

    void Physics::Initialize()
    {
        const EngineSettings& engineSettings = Settings::GetEngineSettings();

        if (engineSettings.physicsEnabled)
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
