#pragma once

#include "QF_QKey.h"

namespace QwerkE {

    namespace Input {

        enum InputContexts : uint8_t
        {
            e_None = 0,
            e_InGame,
            e_PauseMenu,
            e_All
        };

        InputContexts CurrentContext();
        void ChangeCurrentContext(const InputContexts a_NewInputContext);

        using ContextChangedCallback = std::function<void(const InputContexts a_NewInputContext)>;
        void OnContextChange(const ContextChangedCallback& a_Callback);
        void StopContextChange(const ContextChangedCallback& a_FuncId);

        void SendSystemInput(u16 a_KeyCode, bool a_KeyDown, u16 a_WScanCode = 0);
        void SendSystemInput(QKey a_Key, bool a_KeyDown);

    }

}
