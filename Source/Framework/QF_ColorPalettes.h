#pragma once

#include <vector>

#include "QF_Colors.h"

namespace QwerkE {

    namespace Colors {

        namespace Palettes {

            namespace UI {

                inline Color4 Primary() { return Color4(0.2f, 0.6f, 1.0f, 1.0f); }
                inline Color4 Success() { return Color4(0.2f, 0.8f, 0.3f, 1.0f); }
                inline Color4 Warning() { return Color4(1.0f, 0.7f, 0.2f, 1.0f); }
                inline Color4 Danger() { return Color4(0.9f, 0.2f, 0.2f, 1.0f); }
                inline Color4 Disabled() { return Color4(0.5f, 0.5f, 0.5f, 0.6f); }

                inline Color4 Hover(const Color4& base)
                {
                    return Lerp(base, Color4(1.f, 1.f, 1.f, 1.f), 0.15f);
                }

                inline Color4 Active(const Color4& base)
                {
                    return Lerp(base, Color4(0.f, 0.f, 0.f, 0.f), 0.2f);
                }

            } // namespace UI

            inline Color4 Simple(int index)
            {
                static std::vector<Color4> simplePalette = {
                    Color4(231.f, 76.f, 60.f, 1.0f),   // red
                    Color4(52.f, 152.f, 219.f, 1.0f),  // blue
                    Color4(46.f, 204.f, 113.f, 1.0f),  // green
                    Color4(155.f, 89.f, 182.f, 1.0f),  // purple
                    Color4(241.f, 196.f, 15.f, 1.0f),  // yellow
                    Color4(230.f, 126.f, 34.f, 1.0f),  // orange
                    Color4(26.f, 188.f, 156.f, 1.0f)   // teal
                };
                return simplePalette[index % simplePalette.size()];
            }

            inline Color4 Default(int index)
            {
                static std::vector<Color4> defaultPalette = {
                    // Reds → Oranges
                    { 0.90f, 0.25f, 0.25f, 1.0f }, { 0.90f, 0.35f, 0.25f, 1.0f },
                    { 0.90f, 0.45f, 0.25f, 1.0f }, { 0.90f, 0.55f, 0.25f, 1.0f },
                    { 0.90f, 0.65f, 0.25f, 1.0f },

                    // Oranges → Yellows
                    { 0.90f, 0.75f, 0.25f, 1.0f }, { 0.85f, 0.85f, 0.25f, 1.0f },
                    { 0.75f, 0.85f, 0.25f, 1.0f }, { 0.65f, 0.85f, 0.25f, 1.0f },
                    { 0.55f, 0.85f, 0.25f, 1.0f },

                    // Yellow → Greens
                    { 0.45f, 0.85f, 0.25f, 1.0f }, { 0.35f, 0.85f, 0.25f, 1.0f },
                    { 0.25f, 0.85f, 0.25f, 1.0f }, { 0.25f, 0.85f, 0.35f, 1.0f },
                    { 0.25f, 0.85f, 0.45f, 1.0f },

                    // Greens → Teals
                    { 0.25f, 0.85f, 0.55f, 1.0f }, { 0.25f, 0.85f, 0.65f, 1.0f },
                    { 0.25f, 0.85f, 0.75f, 1.0f }, { 0.25f, 0.85f, 0.85f, 1.0f },
                    { 0.25f, 0.75f, 0.85f, 1.0f },

                    // Teals → Blues
                    { 0.25f, 0.65f, 0.85f, 1.0f }, { 0.25f, 0.55f, 0.85f, 1.0f },
                    { 0.25f, 0.45f, 0.85f, 1.0f }, { 0.25f, 0.35f, 0.85f, 1.0f },
                    { 0.25f, 0.25f, 0.85f, 1.0f },

                    // Blues → Purples
                    { 0.35f, 0.25f, 0.85f, 1.0f }, { 0.45f, 0.25f, 0.85f, 1.0f },
                    { 0.55f, 0.25f, 0.85f, 1.0f }, { 0.65f, 0.25f, 0.85f, 1.0f },
                    { 0.75f, 0.25f, 0.85f, 1.0f },

                    // Purples → Magentas
                    { 0.85f, 0.25f, 0.85f, 1.0f }, { 0.85f, 0.25f, 0.75f, 1.0f },
                    { 0.85f, 0.25f, 0.65f, 1.0f }, { 0.85f, 0.25f, 0.55f, 1.0f },
                    { 0.85f, 0.25f, 0.45f, 1.0f }
                };
                return defaultPalette[index % defaultPalette.size()];
            }

            inline Color4 DefaultMuted(int index)
            {
                static std::vector<Color4> defaultMutedPalette = {
                    { 0.70f, 0.30f, 0.30f, 1.0f }, { 0.70f, 0.50f, 0.30f, 1.0f },
                    { 0.70f, 0.70f, 0.30f, 1.0f }, { 0.50f, 0.70f, 0.30f, 1.0f },
                    { 0.30f, 0.70f, 0.30f, 1.0f },

                    { 0.30f, 0.70f, 0.50f, 1.0f }, { 0.30f, 0.70f, 0.70f, 1.0f },
                    { 0.30f, 0.50f, 0.70f, 1.0f }, { 0.30f, 0.30f, 0.70f, 1.0f },
                    { 0.50f, 0.30f, 0.70f, 1.0f },

                    { 0.70f, 0.30f, 0.70f, 1.0f }, { 0.70f, 0.30f, 0.50f, 1.0f },
                    { 0.70f, 0.40f, 0.40f, 1.0f }, { 0.60f, 0.60f, 0.30f, 1.0f },
                    { 0.40f, 0.60f, 0.60f, 1.0f }
                };
                return defaultMutedPalette[index % defaultMutedPalette.size()];
            }

        } // namespace Palettes

    } // namespace Colors

} // namespace QwerkE
