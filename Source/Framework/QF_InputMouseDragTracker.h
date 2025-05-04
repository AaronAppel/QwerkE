#pragma once

// Reference: https://www.gamedev.net/blogs/entry/2250186-designing-a-robust-input-handling-system-for-games/

#include "QF_QKey.h"
#include "QF_Input.h"

namespace QwerkE {

    namespace Input {

        struct MouseDragTracker
        {
            MouseDragTracker() = default;
            MouseDragTracker(QKey keyToTrack) { m_KeyToTrack = keyToTrack; }

            void Update()
            {
                if (IsActive())
                {
                    m_FrameDelta = m_LatestPosition;
                    m_LatestPosition = Input::MousePos();
                    m_FrameDelta = m_FrameDelta - m_LatestPosition;

                    if (!m_IsActive)
                    {
                        m_StartingPosition = m_LatestPosition;
                        m_IsActive = true;
                        m_FrameDelta = vec2f(0.f, 0.f);
                    }
                }
                else
                {
                    m_IsActive = false;
                    m_FrameDelta = vec2f(0.f, 0.f);
                }
            }

            const vec2f& MouseStartDelta() const { return m_StartingPosition - m_LatestPosition; }
            const vec2f& MouseFrameDelta() const { return m_FrameDelta; }

            bool m_IsActive = false;
            bool IsActive() { return Input::MouseDown(m_KeyToTrack); }

            QKey m_KeyToTrack = QKey::e_MouseButton1;

            vec2f m_StartingPosition;
            vec2f m_LatestPosition;
            vec2f m_FrameDelta;
        };

    }

}
