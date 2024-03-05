#include "QF_Debugger.h"

#ifdef _QDebug

#include "QF_Log.h"
#include "QF_Graphics_Header.h"

namespace QwerkE {

    bool Debugger::m_ConsoleIsOpen = false;

    void Debugger::ToggleConsole()
    {
        m_ConsoleIsOpen = !m_ConsoleIsOpen;
        if (m_ConsoleIsOpen)
        {
            LOG_INFO("Console opened");
        }
        else
        {
            LOG_INFO("Console closed");
        }
    }

    void Debugger::SetConsoleVisible(bool visibility)
    {
        m_ConsoleIsOpen = visibility;
        if (m_ConsoleIsOpen)
        {
            LOG_INFO("Console opened");
        }
        else
        {
            LOG_INFO("Console closed");
        }
    }

    // TODO: What should the debugger handle vs other systems?
    //     Systems should manage themselves, but expose API for debugger to communicate/control debug behaviour.
    void Debugger::CheckGraphicsErrors()
    {
        // #TODO Check graphics errors
    }

    void Debugger::CheckAudioErrors()
    {
        // TODO: Check Audio system error state
    }

}
#endif
