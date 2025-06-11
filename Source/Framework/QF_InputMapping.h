#pragma once

#include "QF_Input.h"

#include "QE_Input.h"

namespace QwerkE {

    namespace Input {

        // #TODO Review and resolve comments
        // Additional considerations //
        // Input layering, consumption, or otherwise avoiding/ignoring input based on context such as focused windows and gameplay state(s)
        // Example contexts: Editor mode start scene button can then become the stop scene button when the context change to play mode.
        // Same button/key combo, different context-dependent action.

        // EXPERIMENTAL InputHandler class (mapping and state management like enabled or game paused)
        // Input::InputHandler m_Handler;
        // keyMappings = {
        //     { "Jump", QKey::A },
        //     { "MoveLeft", QKey::Left }
        //     { "MoveLeft", QGamepad::e_GamepadAxis01 }
        // };
        // m_Handler->SetKeyMappings(keyMappings);
        // handlerContexts = { "GamePlaying" /*, "GamePaused"*/ };
        // m_Handler->SetContexts(handlerContexts);
        // m_Handler->RegisterEventCallbacks(true); // #NOTE To automatically disable on game events like game paused, using assigned contexts
        // m_Handler->SetEnabled(true);
        // if (m_Handler->Activated("Jump")) { player.Jump(); }
        // if (m_Handler->IsActivate("Walk")) { player.pos += player.forward * player.walkSpeed; }
        // if (m_Handler->Deactivate("Move")) { player.isSprinting = false; }

        // Maybe fun to have but really just extra. Behaves like registering and unregistering plus counting.
        // Really just to avoid user from managing state, but at what cost?
        // void Input::InputHandlerStartPressCounter(int key) { }
        // u16 Input::InputHandlerGetPressCounter(int key) { return 0; }
        // void Input::InputHandlerStopPressCounter(int key) { }

        // InputDevice object as handler?

        class InputMapping
        {
        public:
            InputMapping() = default;

            InputMapping(std::vector<QKey>& a_Keys)
            {
                m_Keys = a_Keys;
            }

            bool IsActive()
            {
                for (size_t i = 0; i < m_Keys.size(); i++)
                {
                    if (!Input::KeyDown(m_Keys[i]))
                    {
                        return false;
                    }
                }
                return true;
            }

        protected:
            bool m_IsEnabled = true;

        private:
            std::vector<QKey> m_Keys;
            bool m_LastStateIsActive = false;
        };

        class InputMappingContextual : public InputMapping
        {
        public:
            InputMappingContextual()
            {
                Initialize({ e_All });
            }

            InputMappingContextual(const std::vector<InputContexts>& a_Contexts)
            {
                Initialize(a_Contexts);
            }

            ~InputMappingContextual()
            {
                // #TODO Need to support class instance function registration
                // StopContextChange(OnInputContextChanged);
            }

        private:
            void Initialize(const std::vector<InputContexts>& a_Contexts)
            {
                InputContexts currentSystemContext = CurrentContext();
                m_IsEnabled = false;

                for (size_t i = 0; i < a_Contexts.size(); i++)
                {
                    if (a_Contexts[i] == e_All)
                    {
                        m_Contexts.clear();
                        m_Contexts.emplace_back(e_All);
                        m_IsEnabled = true;
                        break;
                    }
                    else if (a_Contexts[i] == currentSystemContext)
                    {
                        m_IsEnabled = true;
                    }
                }

                // #TODO Need to support class instance function registration
                // OnContextChange(OnInputContextChanged);
            }

            void OnInputContextChanged(const InputContexts a_NewContext)
            {
                // #TODO Check if new context is valid
                m_IsEnabled = false;
                for (size_t i = 0; i < m_Contexts.size(); i++)
                {
                    if (m_Contexts[i] == a_NewContext)
                    {
                        m_IsEnabled = true;
                        break;
                    }
                }
            }

            std::vector<InputContexts> m_Contexts;
        };

    }

}
