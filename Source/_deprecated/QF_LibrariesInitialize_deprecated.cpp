#include "QF_LibrariesInitialize.h"

#include "QF_LibrariesInclude.h"

#include "QF_Debug.h"
#include "QF_Defines.h"
#include "QF_Platform.h"
#include "QF_Log.h"
#include "QF_ConfigHelper.h"

namespace QwerkE {

    static bool s_IsInitialized = false;

    eEngineMessage Libs_Setup()
    {
        // #TODO This logic tests to see if libraries can load, and then unloads them.
        // This logic should exist in other systems.
        // There may be some benefit to having test cases to see when a library fails to load,
        // but that can be done in the specific system that uses it.

        ASSERT(!s_IsInitialized, "Libraries are already initialized!");

        ConfigData configData = ConfigHelper::GetConfigData();
        // if (configData.systems.AudioEnabled) {}

        eEngineMessage returnMessage = eEngineMessage::_QSuccess;
        LOG_TRACE("{0}: Initializing libraries...", __FUNCTION__);


        s_IsInitialized = returnMessage == eEngineMessage::_QSuccess;
        if (s_IsInitialized)
        {
            LOG_TRACE("{0}: Libraries initialized successfully", __FUNCTION__);
        }
        else
        {
            LOG_CRITICAL("{0}: Error loading libraries!", __FUNCTION__);
        }

        return returnMessage;
    }

    eEngineMessage Libs_TearDown()
    {

        s_IsInitialized = false;

        return eEngineMessage::_QSuccess;
    }

}
