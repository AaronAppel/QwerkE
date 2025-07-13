#pragma once

namespace QwerkE {

    struct ComponentText
    {
        ComponentText() = default;

        // #TODO On screen text handling
        // Review font.cpp from bgfx examples solution.
        // Will need font management in Assets:: including loading and helpers to push/pop.
        // Also want ImGui to be able to use it, and may as well add font icons for ImGui to ues as well.
    };

}
